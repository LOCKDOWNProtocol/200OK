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
	
	FInputBindingDelegate InputBindingDelegate;

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


public: // 아이템 관련 속성
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* ItemComp;


public: // 컴포넌트화 시키기
	UPROPERTY(EditAnywhere)
	class UMoveComponent* MoveComp;
	UPROPERTY(EditAnywhere)
	class UInteractionComponent* InteractionComp;


public: // Tablet 관련
	bool isTakingTablet = false;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UChildActorComponent* TabletComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ATablet> TabletFactory;


};
