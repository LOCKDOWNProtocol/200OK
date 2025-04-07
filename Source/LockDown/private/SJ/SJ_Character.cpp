#include "SJ/SJ_Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "SJ/SJ_TestItem.h"
#include "SJ/SJ_TestButton.h"
#include "Components/SceneComponent.h"
#include "SJ/SJ_PlayerAnimInstance.h"

ASJ_Character::ASJ_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(40, 0, 80));
	SpringArm->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArm);
	CameraComp->SetRelativeLocation(FVector(30, 0, 0));
	CameraComp->SetRelativeRotation(FRotator(-25, 0, 0));
	CameraComp->bUsePawnControlRotation = false;

	ItemComp=CreateDefaultSubobject<USceneComponent>(TEXT("ItemComp"));
	ItemComp->SetupAttachment(GetMesh(), TEXT("ItemPos"));
	ItemComp->SetRelativeLocation(FVector(-9, 6, 1));

	// 플레이어 컨트롤러 로테이션
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshTmp(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (MeshTmp.Succeeded()) {
		GetMesh()->SetSkeletalMesh(MeshTmp.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void ASJ_Character::BeginPlay()
{
	Super::BeginPlay();
	
	// IMC 연결
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc) {
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys) {
			subSys->AddMappingContext(IMC_SJ, 0);
		}
	}

	// 초기속도를 걷기로 설정
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ASJ_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Input 값을 컨트롤러 회전값에 맞춰 바꿔주기
	PlayerDirection = FTransform(GetControlRotation()).TransformVector(PlayerDirection);
	// 플레이어 이동시키기
	AddMovementInput(PlayerDirection);
	PlayerDirection = FVector::ZeroVector;
}

void ASJ_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// EnhancedInputComponent로 캐스팅
	auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (playerInput) {
		playerInput->BindAction(IA_SJ_LookUp, ETriggerEvent::Triggered, this, &ThisClass::InputLookUp);
		playerInput->BindAction(IA_SJ_Turn, ETriggerEvent::Triggered, this, &ThisClass::InputTurn);
		playerInput->BindAction(IA_SJ_Move, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
		playerInput->BindAction(IA_SJ_Run, ETriggerEvent::Started, this, &ThisClass::InputRun);
		playerInput->BindAction(IA_SJ_Run, ETriggerEvent::Completed, this, &ThisClass::InputRun);
		playerInput->BindAction(IA_SJ_Jump, ETriggerEvent::Started, this, &ThisClass::InputJump);

		playerInput->BindAction(IA_SJ_WalkHold, ETriggerEvent::Started, this, &ThisClass::InputWalkHold);
		playerInput->BindAction(IA_SJ_WalkHold, ETriggerEvent::Completed, this, &ThisClass::InputUnWalkHold);
		playerInput->BindAction(IA_SJ_WalkToggle, ETriggerEvent::Started, this, &ThisClass::InputWalkToggle);

		playerInput->BindAction(IA_PrimaryAction, ETriggerEvent::Started, this, &ThisClass::InputPrimaryAction);
		playerInput->BindAction(IA_SecondaryAction, ETriggerEvent::Started, this, &ThisClass::InputSecondaryAction);

		playerInput->BindAction(IA_ReleaseItem, ETriggerEvent::Started, this, &ThisClass::ReleaseItem);
		
		playerInput->BindAction(IA_Inventory, ETriggerEvent::Started, this, &ThisClass::Inventory);

	}

}

void ASJ_Character::InputTurn(const FInputActionValue& InputValue)
{
	float value = InputValue.Get<float>();
	AddControllerYawInput(value);
}

void ASJ_Character::InputLookUp(const FInputActionValue& InputValue)
{
	float value = InputValue.Get<float>();
	AddControllerPitchInput(value);
}

void ASJ_Character::InputMove(const struct FInputActionValue& InputValue)
{
	FVector2D value = InputValue.Get<FVector2D>();
	PlayerDirection.X = value.X;
	PlayerDirection.Y = value.Y;
}

void ASJ_Character::InputRun()
{
	auto movement = GetCharacterMovement();
	if (!movement)return;
	if (movement->MaxWalkSpeed > WalkSpeed) {
		movement->MaxWalkSpeed = WalkSpeed;
	}
	else {
		movement->MaxWalkSpeed = RunSpeed;
	}
}


void ASJ_Character::InputJump(const struct FInputActionValue& InputValue)
{
	Jump();
}

void ASJ_Character::InputWalkHold()
{
	Crouch();
}

void ASJ_Character::InputUnWalkHold()
{
	UnCrouch();
}

void ASJ_Character::InputWalkToggle()
{
	bCrouched = !bCrouched;
	bCrouched ? Crouch() : UnCrouch();
}

void ASJ_Character::InputPrimaryAction()
{
	// 장비 중이면 아이템 휘두르기
	if (bHasItem) {
		AttackItem();
		return;
	}

	// 맨손 일 때 아이템인지 버튼인지 먼저 판별
	FHitResult HitResult;
	FVector StartPos = CameraComp->GetComponentLocation();
	FVector EndPos = StartPos + CameraComp->GetForwardVector() * 300.f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, 1.0f, 0, 1.0f);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECC_Visibility, Params);
	if (!bHit)return;
	AActor* HitActor = HitResult.GetActor();

	// 아이템이라면 줍고 버튼이라면 누르기
	if (Cast<ASJ_TestItem>(HitActor)) {
		PickupItem(HitActor);
	}
	else if (Cast<ASJ_TestButton>(HitActor)) {
		PressButton(HitActor);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, TEXT("Cannot Interaction Actor"));
	}

}

void ASJ_Character::PickupItem(AActor* HitActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("Item Casting Success"));

	// AnimInstance -> bHasItem = true로 변경
	if (USJ_PlayerAnimInstance* AnimInst = Cast<USJ_PlayerAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInst->bHasItem = true;
	}
	// 아이템 소유 표시
	bHasItem = true;
	ownedItem = HitActor;
	// 소켓에 붙이기
	if (UStaticMeshComponent* ItemMesh = HitActor->FindComponentByClass<UStaticMeshComponent>())
	{
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ItemMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("ItemPos"));
	}
}

void ASJ_Character::PressButton(AActor* HitActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Button Casting Success"));

	// 버튼 누르기
}

void ASJ_Character::AttackItem()
{
	// 근접공격 : 장비중일 때 아이템으로 휘두르기
	if (!bHasItem || !ownedItem) return;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Item Attack!"));
}

void ASJ_Character::ReleaseItem()
{
	// 장비중일 때 아이템 버리기
	if (!bHasItem || !ownedItem) return;

	// 라인트레이스로 mesh에 붙인 아이템을 버리기
	// 소유off + Detach + 물리on + 콜리전처리
	if ( UStaticMeshComponent* ItemMesh=ownedItem->FindComponentByClass<UStaticMeshComponent>() )
	{
		ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetCollisionObjectType(ECC_PhysicsBody);
	}

	bHasItem = false;
	ownedItem = nullptr;

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue, TEXT("Release Item!"));
}

void ASJ_Character::Inventory()
{
	// E key 누르면 아이템을 인벤토리에 보관하기
}

void ASJ_Character::InputSecondaryAction()
{
	// Mouse R 액션
}
