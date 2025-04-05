#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SJ_Character.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UEnhancedInputComponent*)

UCLASS()
class LOCKDOWN_API ASJ_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ASJ_Character();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private: // SpringArm
	UPROPERTY(VisibleAnywhere, Category="SpringArm")
	class USpringArmComponent* SpringArm;


public: // Camera
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
    class UCameraComponent* CameraComp;


private: // Input
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputMappingContext* IMC_SJ;
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

private: // 변수
	UPROPERTY(EditDefaultsOnly, Category=PlayerSpeed)
	float WalkSpeed = 200.f;	
	UPROPERTY(EditDefaultsOnly, Category=PlayerSpeed)
	float RunSpeed = 600.f;

	FVector PlayerDirection;
	bool bCrouched;
	
private: // Input 관련 함수
	void InputTurn(const struct FInputActionValue& InputValue);
	void InputLookUp(const struct FInputActionValue& InputValue);
	void InputMove(const struct FInputActionValue& InputValue);
	void InputRun();
	void InputJump(const struct FInputActionValue& InputValue);
	void InputWalkHold();
	void InputUnWalkHold();
	void InputWalkToggle();
};
