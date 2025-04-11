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

	
protected:
	virtual void StartMission() {};
	virtual int32 ClearMission() { return 0; };
	void EndMission();

	FORCEINLINE void SetTotalCount(int32 t) { totalCount = t; }
	FORCEINLINE void SetClearCount(int32 c) { clearCount = c; }

private:
	bool bIsClear = false;
	int32 totalCount;
	int32 clearCount = 0;

};
