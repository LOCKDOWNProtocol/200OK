// Copyright Epic Games, Inc. All Rights Reserved.

#include "LockDownGameMode.h"
#include "SJ/LockDownCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALockDownGameMode::ALockDownGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
