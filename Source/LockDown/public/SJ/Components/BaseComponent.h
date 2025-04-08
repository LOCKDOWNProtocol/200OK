#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOCKDOWN_API UBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseComponent();

protected:
	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public: // 컴포넌트 소유 액터
	UPROPERTY()
	class ASJ_Character* me;

	virtual void SetupInputBinding(class UEnhancedInputComponent* Input);

};
