// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MW/Items.h"
#include "Knife.generated.h"

/**
 * 
 */
UCLASS()
class AKnife : public AItems
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKnife();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DefaultSetting")
	class UStaticMeshComponent* MeshComp;

};

