#include "SJ/Components/MoveComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedPlayerInput.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "SJ/SJ_Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UMoveComponent::UMoveComponent()
{
	PrimaryComponentTick.bCanEverTick=true;

}

void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	MoveComp=me->GetCharacterMovement();
	MoveComp->MaxWalkSpeed=WalkSpeed;
}

void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Input 값을 컨트롤러 회전값에 맞춰 바꿔주기
	PlayerDirection=FTransform(me->GetControlRotation()).TransformVector(PlayerDirection);
	// 플레이어 이동시키기
	me -> AddMovementInput(PlayerDirection);
	PlayerDirection=FVector::ZeroVector;

	// 카메라 각도 클램프
	FRotator ControlRot=me->GetControlRotation();
	ControlRot.Pitch=FMath::ClampAngle(ControlRot.Pitch, MinPitch, MaxPitch);
	me->GetController()->SetControlRotation(ControlRot);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Current Clamp : Pitch = %.2f"), ControlRot.Pitch));
}

void UMoveComponent::SetupInputBinding(class UEnhancedInputComponent* Input)
{
	Super::SetupInputBinding(Input);

	Input->BindAction(IA_SJ_LookUp, ETriggerEvent::Triggered, this, &ThisClass::InputLookUp);
	Input->BindAction(IA_SJ_Turn, ETriggerEvent::Triggered, this, &ThisClass::InputTurn);
	Input->BindAction(IA_SJ_Move, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
	Input->BindAction(IA_SJ_Run, ETriggerEvent::Started, this, &ThisClass::InputRun);
	Input->BindAction(IA_SJ_Run, ETriggerEvent::Completed, this, &ThisClass::InputRun);
	Input->BindAction(IA_SJ_Jump, ETriggerEvent::Started, this, &ThisClass::InputJump);
	Input->BindAction(IA_SJ_WalkHold, ETriggerEvent::Started, this, &ThisClass::InputWalkHold);
	Input->BindAction(IA_SJ_WalkHold, ETriggerEvent::Completed, this, &ThisClass::InputUnWalkHold);
	Input->BindAction(IA_SJ_WalkToggle, ETriggerEvent::Started, this, &ThisClass::InputWalkToggle);

}

void UMoveComponent::InputTurn(const FInputActionValue& InputValue)
{
	float value=InputValue.Get<float>();
	me->AddControllerYawInput(value);
}

void UMoveComponent::InputLookUp(const FInputActionValue& InputValue)
{
	float value=InputValue.Get<float>();
	me->AddControllerPitchInput(value);
}

void UMoveComponent::InputMove(const struct FInputActionValue& InputValue)
{
	FVector2D value=InputValue.Get<FVector2D>();
	PlayerDirection.X=value.X;
	PlayerDirection.Y=value.Y;
}

void UMoveComponent::InputRun()
{
	auto movement=MoveComp;
	if ( !movement )return;
	if ( movement->MaxWalkSpeed > WalkSpeed ) {
		movement->MaxWalkSpeed=WalkSpeed;
	}
	else {
		movement->MaxWalkSpeed=RunSpeed;
	}
}

void UMoveComponent::InputJump(const struct FInputActionValue& InputValue)
{
	me->Jump();
}

void UMoveComponent::InputWalkHold()
{
	me->Crouch();
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Black, TEXT("Crouch!"));
}

void UMoveComponent::InputUnWalkHold()
{
	me->UnCrouch();
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Black, TEXT("UnCrouch!"));
}

void UMoveComponent::InputWalkToggle()
{
	bCrouched=!bCrouched;
	bCrouched ? me->Crouch() : me->UnCrouch();
}

