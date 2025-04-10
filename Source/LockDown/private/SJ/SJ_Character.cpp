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
#include "SJ/Components/InteractionComponent.h"
#include "Components/ChildActorComponent.h"
#include "MW/Tablet.h"
#include "shDebug.h"

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

	// 컴포넌트들
	MoveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComp"));
	InteractionComp=CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComp"));
	

	TabletComp = CreateDefaultSubobject<UChildActorComponent> (TEXT("Tablet"));
	TabletComp->SetupAttachment (SpringArm);

	TabletComp->SetChildActorClass (TabletFactory);



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
		if ( InputBindingDelegate.IsBound() ) {
			InputBindingDelegate.Broadcast(playerInput);
		}
	}
}

