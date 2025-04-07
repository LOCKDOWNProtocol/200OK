// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MW/Items.h"
#include <utility>
#include "Fuse.generated.h"
using namespace std;

/**
 * 
 */

 UENUM(BlueprintType)
	 enum class PartColor : uint8
 {
		Red		UMETA(DisplayName = "Red"),
		Yellow	UMETA(DisplayName = "Yellow"),
		Blue	UMETA(DisplayName = "Blue")
 };
UCLASS()
class LOCKDOWN_API AFuse : public AItems
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFuse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	pair<PartColor, PartColor> Colors;

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DefaultSetting")
	class UBoxComponent* BoxComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DefaultSetting")
	class UStaticMeshComponent* FirstMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DefaultSetting")
	class UStaticMeshComponent* SecondMesh;

	TArray<UMaterial*> MatColors;
	TMap<PartColor, UMaterial*> ColorMap;

	FORCEINLINE pair<PartColor, PartColor> GetColors() { return Colors; }
	FORCEINLINE void SetColors(pair<PartColor, PartColor> value) { Colors = value; }
};
