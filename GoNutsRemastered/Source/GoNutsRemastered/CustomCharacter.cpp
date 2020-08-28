// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacter.h"
#include "LevelChunk.h"
#include "LevelGenerator.h"
//#include "PlayerMoveState.h"
//#include "PlayerDefaultMoveState.h"

DEFINE_LOG_CATEGORY(LogCustomCharacter);

// Sets default values
ACustomCharacter::ACustomCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACustomCharacter::init(ALevelGenerator* levelGen)
{
	if (IsValid(levelGen))
	{
		_levelGen = levelGen;
	}
	else
	{
		UE_LOG(LogCustomCharacter, Error, TEXT("Invalid level generator reference passed to the player!"));
	}
}

ALevelChunk* ACustomCharacter::getChunk() const
{
	return _chunk;
}

// Called when the game starts or when spawned
void ACustomCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACustomCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ACustomCharacter::getIsRotating() const
{
	return _rotatePlayer;
}

