﻿#include "SJ/SJ_PlayerAnimInstance.h"
#include "SJ/SJ_Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void USJ_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void USJ_PlayerAnimInstance::PlayPressButtonAnim()
{
	if ( PressButtonMontage == nullptr ) return;
	Montage_Play(PressButtonMontage);
}
