﻿#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SJ_PlayerAnimInstance.generated.h"

UCLASS()
class LOCKDOWN_API USJ_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public: // 매 프레임 갱신되는 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasItem = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	
public:
	// 재생할 공격 애니메이션 몽타주
	UPROPERTY( EditDefaultsOnly, Category = TPSPlayerAnim )
	class UAnimMontage* PressButtonMontage;

	// 공격 애니메이션 재생 함수
	void PlayPressButtonAnim();


};
