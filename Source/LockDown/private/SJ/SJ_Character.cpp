#include "SJ/SJ_Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "MW/Items.h"
#include "SJ/SJ_TestItem.h"
#include "SJ/SJ_TestButton.h"
#include "Components/SceneComponent.h"
#include "SJ/SJ_PlayerAnimInstance.h"
#include "SJ/Components/MoveComponent.h"

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
	ItemComp->SetRelativeLocation(FVector(-10.f, 3.f, 1.f));
	ItemComp->SetRelativeRotation(FRotator(-90.f, 0.f, -180.f));

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

	// 컴포넌트화
	MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComp"));


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
}

void ASJ_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASJ_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// EnhancedInputComponent로 캐스팅
	auto playerInput=Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (playerInput) {

		// 컴포넌트화	
		MoveComp->SetupInputBinding(playerInput);
	
		playerInput->BindAction(IA_PrimaryAction, ETriggerEvent::Started, this, &ThisClass::InputPrimaryAction);
		playerInput->BindAction(IA_SecondaryAction, ETriggerEvent::Started, this, &ThisClass::InputSecondaryAction);

		playerInput->BindAction(IA_ReleaseItem, ETriggerEvent::Started, this, &ThisClass::ReleaseItem);
		
		playerInput->BindAction(IA_Inventory, ETriggerEvent::Started, this, &ThisClass::Inventory);

		playerInput->BindAction(IA_Tablet, ETriggerEvent::Started, this, &ThisClass::TakeTablet);

	}

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
	FVector EndPos = StartPos + CameraComp->GetForwardVector() * TraceLength;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, 1.0f, 0, 1.0f);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECC_Visibility, Params);
	if (!bHit)return;
	AActor* HitActor = HitResult.GetActor();

	// 아이템이라면 줍고 버튼이라면 누르기
	if ( Cast<AItems>(HitActor) ) {
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
	if ( bHasTablet )return;

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

void ASJ_Character::ReleaseItem()
{
	// 장비중일 때 아이템 버리기
	if ( !bHasItem || !ownedItem || bHasTablet) return;

	// 라인트레이스로 mesh에 붙인 아이템을 버리기
	// 소유off + Detach + 물리on + 콜리전처리
	if ( UStaticMeshComponent* ItemMesh=ownedItem->FindComponentByClass<UStaticMeshComponent>() )
	{
		ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetCollisionObjectType(ECC_PhysicsBody);
	}

	// AnimInstance -> bHasItem = false로 변경
	if ( USJ_PlayerAnimInstance* AnimInst=Cast<USJ_PlayerAnimInstance>(GetMesh()->GetAnimInstance()) )
	{
		AnimInst->bHasItem=false;
	}

	bHasItem=false;
	ownedItem=nullptr;

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue, TEXT("Release Item!"));
}

void ASJ_Character::PressButton(AActor* HitActor)
{

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Button Casting Success"));

	// 테스트용 거리 : 버튼과의 거리가 1m 이상일 경우 누르지 못함
	float Distance = FVector::Dist(HitActor->GetActorLocation(), this->GetActorLocation());
	if ( Distance > 100.f ) return;
	
	// 버튼 누르기
	if ( USJ_PlayerAnimInstance* AnimInst=Cast<USJ_PlayerAnimInstance>(GetMesh()->GetAnimInstance()) )
	{
		AnimInst->PlayPressButtonAnim();
	}
}

void ASJ_Character::AttackItem()
{
	// 근접공격 : 장비중일 때 아이템으로 휘두르기
	if (!bHasItem || !ownedItem || bHasTablet) return;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Item Attack!"));
}

void ASJ_Character::Inventory()
{
	// Todo. E key 누르면 아이템을 인벤토리에 보관
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("E key : Inventory"));
}

void ASJ_Character::InputSecondaryAction()
{
	// Todo. Mouse R 액션
	// 일단 아이템 착용 중이 아닐땐 리턴으로만 테스트
	if ( !bHasItem || !ownedItem ) return;

	// 아이템 착용중일 경우 R->L Input 받으면 특정 위치에 두울 수 있다

}

void ASJ_Character::TakeTablet()
{
	// Todo. 태블릿 들고 이동도 가능해야함, 아이템 줍기, 공격 등은 불가능
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("Q key : TakeTablet"));
}
