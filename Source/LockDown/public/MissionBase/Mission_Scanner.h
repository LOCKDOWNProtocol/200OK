// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MissionBase/MissionBase.h"
#include "Mission_Scanner.generated.h"

/**
 * 
 */
UCLASS()
class LOCKDOWN_API AMission_Scanner : public AMissionBase
{
	GENERATED_BODY()
	

public:
	AMission_Scanner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


};
