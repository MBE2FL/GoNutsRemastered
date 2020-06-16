// Fill out your copyright notice in the Description page of Project Settings.


#include "PedestrianManager.h"
#include "GameFramework/Character.h"
#include "LevelGenerator.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPedestrianManager::UPedestrianManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPedestrianManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	// Attempt to find a reference to the level generator.
	TSubclassOf<ALevelGenerator> levelGenClass;
	ALevelGenerator* levelGenerator;

	levelGenerator = Cast<ALevelGenerator>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelGenerator::StaticClass()));
	// Destroy this component if the level generator could not be found.
	if (!levelGenerator)
	{
		UE_LOG(LogTemp, Error, TEXT("Pedestrian manager failed to find level generator!"));
		DestroyComponent();
	}
	else
	{
		levelGenerator->onCrosswalkSpawned().AddDynamic(this, &UPedestrianManager::spawnPedestrians);
	}
}


// Called every frame
void UPedestrianManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPedestrianManager::spawnPedestrians(ALevelSegment* crosswalk)
{
	UE_LOG(LogTemp, Warning, TEXT("Level gen notified pedstrian manager about crosswalk."));
}

