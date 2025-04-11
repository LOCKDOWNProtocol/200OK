#include "MissionBase/MissionBase.h"

AMissionBase::AMissionBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMissionBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMissionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMissionBase::EndMission()
{
	if(totalCount == clearCount )
		bIsClear = true;
}

