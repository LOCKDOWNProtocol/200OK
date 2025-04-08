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


	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_PrimaryAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_SecondaryAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_ReleaseItem;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_Inventory;	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* IA_Tablet;


public: // 아이템 관련 속성
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* ItemComp;


private: // Input 관련 함수


	void InputPrimaryAction();
	void ReleaseItem();
	void Inventory();
	void TakeTablet();
	void InputSecondaryAction();
	

public: // Mouse L - PrimaryAction 관련
	bool bHasItem = false;
	UPROPERTY()
	AActor* ownedItem = nullptr;
	float TraceLength = 200.f;
	void PickupItem(AActor* HitActor);
	void PressButton(AActor* HitActor);
	void AttackItem();


public: // Mouse R - SecondaryAction 관련
	bool bHasTablet = false;


public: // 컴포넌트화 시키기
	UPROPERTY(EditAnywhere)
	class UMoveComponent* MoveComp;

	
};
