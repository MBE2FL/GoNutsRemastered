// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenState.h"
#include "FreeRoamCharacter.h"


DEFINE_LOG_CATEGORY(LogLevelGenState);


void ULevelGenState::init(ALevelGenerator* levelGen)
{
	if (IsValid(levelGen))
	{
		_levelGen = levelGen;
		_player = _levelGen->getPlayer();
		_initialized = true;
	}
	else
	{
		UE_LOG(LogLevelGenState, Error, TEXT("Failed to initialize level generator state due to invalid level generator reference!"));
	}
}

void ULevelGenState::cleanupState()
{
	ConditionalBeginDestroy();
}
