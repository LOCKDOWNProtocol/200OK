#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOCKDOWN_API UInteractionComponent : public UBaseComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public: 
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

	virtual void SetupInputBinding(class UEnhancedInputComponent* Input) override;
	void InputPrimaryAction();
	void InputFKey();
	void Inventory();
	void TakeTablet();
	void InputSecondaryAction();
	

public: // Mouse L - PrimaryAction 관련
	bool bHasItem = false;
	bool bHasKnife=false;
	UPROPERTY()
	AActor* ownedItem = nullptr;
	float TraceLength = 200.f;
	void PickupItem(AActor* HitActor);
	void HoldAKnife(AActor* HitActor);
	void PressButton(AActor* HitActor);
	void AttackItem();
	void StabKnife();

public: // F Key 관련
	void ReleaseItem();
	void ReleaseKnife();


public: // Mouse R - SecondaryAction 관련
	bool bHasTablet = false;
	void ThrowItem();

#pragma region Tablet_MW

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ATablet* TabletActor;

	void FindTablet();
	
	

#pragma endregion Tablet_MW

	
};
