// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenState.h"

//ULevelGenState::ULevelGenState(ALevelGenerator* levelGen)
//	: _levelGen(levelGen), _width(levelGen->getWidth())
//{
//}

void ULevelGenState::init(ALevelGenerator* levelGen)
{
	_levelGen = levelGen;
}
