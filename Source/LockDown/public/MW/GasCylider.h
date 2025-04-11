#pragma once

#include "CoreMinimal.h"
#include "MW/Items.h"
#include "GasCylider.generated.h"


UCLASS()
class LOCKDOWN_API AGasCylider : public AItems
{
	GENERATED_BODY()

public:
	AGasCylider();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	int32 PressureLimit; // 근데 이게 노랑, 빨강, 파랑에 따라 다른데 그걸 어떻게 해줘야해?
	FName ColorTag;

public:
	FORCEINLINE int32 GetPressureLimit() { return PressureLimit; }
	FORCEINLINE void SetPressureLimit(int32 value) { PressureLimit = value; }

	FORCEINLINE FName GetColorTag() const { return ColorTag; }
	void Explode();
	
};
