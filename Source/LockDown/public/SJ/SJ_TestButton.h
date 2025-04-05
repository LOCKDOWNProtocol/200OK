#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SJ_TestButton.generated.h"

UCLASS()
class LOCKDOWN_API ASJ_TestButton : public AActor
{
	GENERATED_BODY()
	
public:	
	ASJ_TestButton();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category=TestItem)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, Category=TestItem)
	class UStaticMeshComponent* MeshComp;
};
