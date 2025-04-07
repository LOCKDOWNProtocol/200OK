// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MW/Items.h"
#include "Battery.generated.h"

/**
 * 
 */
UCLASS()
class LOCKDOWN_API ABattery : public AItems
{
	GENERATED_BODY()

	public:
	// Sets default values for this actor's properties
	ABattery();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float ChargePercent;

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DefaultSetting")
	class UBoxComponent* BoxComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DefaultSetting")
	class UStaticMeshComponent* MeshComp;

	FORCEINLINE float GetChargePercent() { return ChargePercent; }
	FORCEINLINE void SetChargePercent(float value) { ChargePercent = value; }
	
};
