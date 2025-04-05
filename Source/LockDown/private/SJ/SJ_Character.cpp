#include "SJ/SJ_Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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