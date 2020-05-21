// Copyright Epic Games, Inc. All Rights Reserved.

#include "OurProjectGameMode.h"
#include "OurProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOurProjectGameMode::AOurProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
