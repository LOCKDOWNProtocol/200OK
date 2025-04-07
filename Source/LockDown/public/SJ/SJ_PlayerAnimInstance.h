#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SJ_PlayerAnimInstance.generated.h"

UCLASS()
class LOCKDOWN_API USJ_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public: // �� ������ ���ŵǴ� �Լ�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasItem = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

};
