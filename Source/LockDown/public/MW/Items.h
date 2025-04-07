// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items.generated.h"

UCLASS()
class LOCKDOWN_API AItems : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItems();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DefaultSettings")
	float AttDamage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DefaultSettings")
	float AttRange;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DefaultSettings")
	float ConsumeStamina;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DefaultSettings")
	bool isTwohandItem;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DefaultSettings")
	class ACharacter* ItemOwner;

#pragma region Getter

protected:
	FORCEINLINE float GetAttDamage() { return AttDamage; }
	FORCEINLINE float GetAttRange()	 { return AttRange;	}
	FORCEINLINE float GetConsumeStamina() { return ConsumeStamina; }
	FORCEINLINE bool GetIsTwohandItem() { return isTwohandItem; }
	FORCEINLINE ACharacter* GetItemOwner() { return ItemOwner; }

#pragma endregion Getter

#pragma region Setter
protected:
	FORCEINLINE void SetAttDamage(float value) { AttDamage = value; }
	FORCEINLINE void SetAttRange(float value) { AttRange = value; }
	FORCEINLINE void SetConsumeStamina(float value) { ConsumeStamina = value; }
	FORCEINLINE void SetIsTwohandItem(bool value){ isTwohandItem = value; }
	FORCEINLINE void SetItemOwner(ACharacter* value) { ItemOwner = value; }
#pragma endregion Setter



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
