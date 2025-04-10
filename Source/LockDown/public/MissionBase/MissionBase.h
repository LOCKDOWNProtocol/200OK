#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionBase.generated.h"

UCLASS()
class LOCKDOWN_API AMissionBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMissionBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
