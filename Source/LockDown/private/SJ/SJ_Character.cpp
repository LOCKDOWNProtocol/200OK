#include "SJ/SJ_Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "SJ/SJ_TestItem.h"
#include "SJ/SJ_TestButton.h"

ASJ_Character::ASJ_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0, 60, 80));
	SpringArm->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArm);
	CameraComp->bUsePawnControlRotation = false;

	// �÷��̾� ��Ʈ�ѷ� �����̼�
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
	
	// IMC ����
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc) {
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys) {
			subSys->AddMappingContext(IMC_SJ, 0);
		}
	}

	// �ʱ�ӵ��� �ȱ�� ����
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ASJ_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Input ���� ��Ʈ�ѷ� ȸ������ ���� �ٲ��ֱ�
	PlayerDirection = FTransform(GetControlRotation()).TransformVector(PlayerDirection);
	// �÷��̾� �̵���Ű��
	AddMovementInput(PlayerDirection);
	PlayerDirection = FVector::ZeroVector;
}

void ASJ_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// EnhancedInputComponent�� ĳ����
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
		playerInput->BindAction(IA_ThrowItem, ETriggerEvent::Started, this, &ThisClass::InputThrowItem);

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
	FHitResult HitResult;
	FVector StartPos = CameraComp->GetComponentLocation();
	FVector EndPos = StartPos + CameraComp->GetForwardVector() * 300.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, 1.0f, 0, 1.0f);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECC_Visibility, Params);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (ASJ_TestItem* Item = Cast<ASJ_TestItem>(HitActor)) {
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("������ �Դϴ�"));
			// ������ �ݱ� ó��
		}
		else if (ASJ_TestButton* Button = Cast<ASJ_TestButton>(HitActor)) {
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue, TEXT("��ư �Դϴ�"));
			// ��ư ������ ó��
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Silver, TEXT("���ͷ��� �Ұ� ����"));
		}
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Hit ����"));
	}
}

void ASJ_Character::InputThrowItem()
{
	// ������� �� ������ ������


}