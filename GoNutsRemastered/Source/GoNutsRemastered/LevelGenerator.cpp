// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "Engine/LevelStreaming.h"
#include "EngineUtils.h"
#include "LevelSegment.h"
#include "LevelGenUpState.h"
#include "LevelGenLeftState.h"

DEFINE_LOG_CATEGORY(LogLevelGen);


ULevelGenUpState* ALevelGenerator::_levelGenUpState = NewObject<ULevelGenUpState>();
ULevelGenLeftState* ALevelGenerator::_levelGenLeftState = NewObject<ULevelGenLeftState>();


// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();


	_levelGenState = NewObject<ULevelGenUpState>();
	_levelGenState->init(this);


	GetWorldTimerManager().SetTimer(_timerHandle, this, &ALevelGenerator::updateLevelGen, 0.5f, true, 1.0f);
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ALevelGenerator::updateLevelGen()
{
	ULevelGenState* currLevelGenState = _levelGenState->updateState();

	// State has changed.
	if (currLevelGenState)
	{

	}
	// Still in previous state.
	else
	{
		_levelGenState->update();
	}
}
