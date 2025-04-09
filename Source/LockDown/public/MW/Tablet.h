// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tablet.generated.h"

UCLASS()
class LOCKDOWN_API ATablet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATablet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DefaultSetting")
	class UArrowComponent* ArrowComp;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DefaultSetting")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DefaultSetting")
	class UWidgetComponent* WidgetComp;

	class ASJ_Character* player;

	bool isTabletOpen = false;

	void SwitchTablet(bool value);

};