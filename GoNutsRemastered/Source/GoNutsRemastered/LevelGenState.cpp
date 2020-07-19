// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenState.h"


DEFINE_LOG_CATEGORY(LogLevelGenState);


void ULevelGenState::init(ALevelGenerator* levelGen)
{
	_levelGen = levelGen;

	if (IsValid(_levelGen))
	{
		_player = _levelGen->getPlayer();
	}
	else
	{
		UE_LOG(LogLevelGenState, Error, TEXT("Failed to initialize level generator state due to invalid level generator reference!"));
	}
}

//void ULevelGenState::cleanupState()
//{
//	ConditionalBeginDestroy();
//}
