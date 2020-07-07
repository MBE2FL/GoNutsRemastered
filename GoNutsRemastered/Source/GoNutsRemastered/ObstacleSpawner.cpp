// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleSpawner.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UObstacleSpawner::UObstacleSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UObstacleSpawner::BeginPlay()
{
	Super::BeginPlay();

	_levelGenerator = Cast<ALevelGenerator>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelGenerator::StaticClass()));

	// Destroy this component if the level generator could not be found.
	if (!_levelGenerator)
	{
		UE_LOG(LogTemp, Error, TEXT("Pedestrian manager failed to find level generator!"));
		DestroyComponent();
	}
	else
	{
		_levelGenerator->onRoadSpawned().AddDynamic(this, &UObstacleSpawner::spawnObstacle);
	}
}


// Called every frame
void UObstacleSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UObstacleSpawner::spawnObstacle(ALevelSegment* road, const TArray<USceneComponent*>& ObstacleSpawnPoints)
{
	UE_LOG(LogTemp, Warning, TEXT("Level gen notified obstacles manager about road."));

	UINT32 ObstaclePicker;
	UINT32 SpawnChance;
	UINT32 SpawnPosPick;
	AActor *tempActor;

	//for (int i = 0; i < ObstacleSpawnPoints.Num(); i++)
	//{
		ObstaclePicker = FMath::RandRange(0, _ObstacleTypes.Num() - 1);
		SpawnChance = FMath::RandRange(0, 5);
		SpawnPosPick = FMath::RandRange(0, ObstacleSpawnPoints.Num() - 1);
		
		if (SpawnChance >= 3)
		{
			tempActor = GetWorld()->SpawnActor(_ObstacleTypes[ObstaclePicker]);
			tempActor->SetActorTransform(ObstacleSpawnPoints[SpawnPosPick]->GetComponentTransform());
		}
		else
		{

		}
	//}


}

