// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MW/Items.h"
#include "GasCylider.generated.h"

/**
 * 
 */
UCLASS()
class LOCKDOWN_API AGasCylider : public AItems
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGasCylider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int32 PressureLimit;

public:
	FORCEINLINE int32 GetPressureLimit() { return PressureLimit; }
	FORCEINLINE void SetPressureLimit(int32 value) { PressureLimit = value; }


	
};
