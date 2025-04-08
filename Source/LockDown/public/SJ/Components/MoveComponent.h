#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOCKDOWN_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMoveComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public: // 컴포넌트 소유 액터
	UPROPERTY()
	class ASJ_Character* me;
	UPROPERTY()
	class UCharacterMovementComponent* MoveComp;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* IA_SJ_LookUp;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* IA_SJ_Turn;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* IA_SJ_Move;	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_SJ_Run;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* IA_SJ_Jump;	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* IA_SJ_WalkHold;	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_SJ_WalkToggle;	



	void InputTurn(const struct FInputActionValue& InputValue);
	void InputLookUp(const struct FInputActionValue& InputValue);
	void InputMove(const struct FInputActionValue& InputValue);
	void InputRun();
	void InputJump(const struct FInputActionValue& InputValue);
	void InputWalkHold();
	void InputUnWalkHold();
	void InputWalkToggle();


	void SetupInputBinding(class UEnhancedInputComponent* Input);


public: // 이동 변수
	UPROPERTY(EditDefaultsOnly, Category=PlayerSpeed)
	float WalkSpeed = 500.f;	
	UPROPERTY(EditDefaultsOnly, Category=PlayerSpeed)
	float RunSpeed = 700.f;
	FVector PlayerDirection;
	bool bCrouched;

};
