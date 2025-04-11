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

void AMissionBase::StartMission()
{
}

int32 AMissionBase::ClearMission()
{
	return 0;
}

