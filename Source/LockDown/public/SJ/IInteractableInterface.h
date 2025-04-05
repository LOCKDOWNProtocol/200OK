#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UIInteractableInterface : public UInterface
{
	GENERATED_BODY()
};


class LOCKDOWN_API IIInteractableInterface
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void OnInteract(AActor* Interactor);
};
