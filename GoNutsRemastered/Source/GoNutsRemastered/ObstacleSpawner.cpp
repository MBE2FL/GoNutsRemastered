// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "LevelGenerator.h"
#include "Obstacle.h"

#if WITH_EDITOR
#include "AssetRegistryModule.h"
#endif

// Sets default values for this component's properties
UObstacleSpawner::UObstacleSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

#if WITH_EDITOR
void UObstacleSpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName propertyName = PropertyChangedEvent.GetPropertyName();

	if (propertyName == GET_MEMBER_NAME_CHECKED(UObstacleSpawner, _refreshObstacleClassTypes))
	{
		if (_refreshObstacleClassTypes)
		{
			UE_LOG(LogLevelGen, Warning, TEXT("Refreshing obstacle class types..."));
			getAllObstacleClassTypes();
			_refreshObstacleClassTypes = false;
		}
	}
}
#endif

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

void UObstacleSpawner::spawnObstacle(ALevelChunk* road, const TArray<USceneComponent*>& ObstacleSpawnPoints)
{
	//UE_LOG(LogTemp, Warning, TEXT("Level gen notified obstacles manager about road."));

	uint32 ObstaclePicker;
	uint32 SpawnChance;
	uint32 SpawnPosPick;
	uint32 SpawnPosPick2;//for if the extra obstacle spawns
	AActor *tempActor;

	ObstaclePicker = FMath::RandRange(0, _GrassObstacleTypes.Num() - 1);
	SpawnChance = FMath::RandRange(0, 5);
	SpawnPosPick = FMath::RandRange(0, ObstacleSpawnPoints.Num() - 1);

	SpawnPosPick2 = SpawnPosPick + 2;
	if (SpawnPosPick2 > static_cast<uint32>(ObstacleSpawnPoints.Num() - 1))
	{
		SpawnPosPick2 -= SpawnPosPick;
	}
		
	if (SpawnChance >= 2)
	{
		switch (ObstaclePicker)
		{
		case 0://case 0 is for the acorn
			for (int i = 0; i < ObstacleSpawnPoints.Num(); i++)
			{
				tempActor = GetWorld()->SpawnActor(_GrassObstacleTypes[ObstaclePicker]);
				tempActor->SetActorTransform(ObstacleSpawnPoints[i]->GetComponentTransform());
			}
			//UE_LOG(LogTemp, Warning, TEXT("It went in here"));
			break;

		default://so far for every other obstacle

			//spawns first obstacle
			tempActor = GetWorld()->SpawnActor(_GrassObstacleTypes[ObstaclePicker]);
			tempActor->SetActorTransform(ObstacleSpawnPoints[SpawnPosPick]->GetComponentTransform());

			//chance to spawn second
			SpawnChance = FMath::RandRange(0, 5);
			if (SpawnChance >= 4)
			{
				ObstaclePicker = FMath::RandRange(1, _GrassObstacleTypes.Num() - 1);//set at 1 - max so no acorn spawns
				tempActor = GetWorld()->SpawnActor(_GrassObstacleTypes[ObstaclePicker]);
				tempActor->SetActorTransform(ObstacleSpawnPoints[SpawnPosPick2]->GetComponentTransform());
			}

			//UE_LOG(LogTemp, Warning, TEXT("It did not go in here"));
			break;
		}
	}

}

#if WITH_EDITOR
void UObstacleSpawner::getAllObstacleClassTypes()
{
	// Load the asset registry module
	FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& assetRegistry = assetRegistryModule.Get();

	TArray<FString> contentPaths;
	contentPaths.Add(TEXT("/Game/LevelChunks/Obstacles"));
	assetRegistry.ScanFilesSynchronous(contentPaths);

	FName baseClassName = AObstacle::StaticClass()->GetFName();

	// Get all derived class names.
	TSet<FName> derivedNames;
	{
		TArray<FName> baseNames;
		baseNames.Add(baseClassName);

		TSet<FName> excluded;
		assetRegistry.GetDerivedClassNames(baseNames, excluded, derivedNames);
	}


	TArray<FName> filterPaths;
	filterPaths.Reserve(3);
	filterPaths.Add(TEXT("/Game/LevelChunks/Obstacles/AllTerrain"));
	filterPaths.Add(TEXT("/Game/LevelChunks/Obstacles/Road"));
	filterPaths.Add(TEXT("/Game/LevelChunks/Obstacles/Grass"));


	for (const FName& filterPath : filterPaths)
	{
		UE_LOG(LogLevelGen, Warning, TEXT("Searching for all obstacle types in %s."), *filterPath.ToString());

		// Filter each sub-folder in the root Obstacles folder.
		FARFilter filter;
		filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
		filter.bRecursiveClasses = true;
		filter.PackagePaths.Add(filterPath);
		filter.bRecursivePaths = true;

		// Load all obstacles, and store them according to their respective folder's designated obstacle type.
		TArray<FAssetData> assetList;
		assetRegistry.GetAssets(filter, assetList);

		FObstacleClassTypes obstacleClassTypes{};
		for (const FAssetData& asset : assetList)
		{
			if (const FString* generatedClassPathPtr = asset.TagsAndValues.Find(TEXT("GeneratedClass")))
			{
				// Convert path to just the name part.
				const FString classObjectPath = FPackageName::ExportTextPathToObjectPath(*generatedClassPathPtr);
				const FString className = FPackageName::ObjectPathToObjectName(classObjectPath);

				// Check if the class is in the derived set.
				if (!derivedNames.Contains(*className))
				{
					continue;
				}

				// Store obstacle according to it's class type.
				TAssetSubclassOf<AObstacle> test = TAssetSubclassOf<AObstacle>(FStringAssetReference(classObjectPath));
				TSubclassOf<AObstacle> obstacleClassType = test.Get();
				UE_LOG(LogLevelGen, Warning, TEXT("Found blueprint: %s"), *test.Get()->GetName());

				obstacleClassTypes._obstacleClassTypes.Add(obstacleClassType);
			}
		}

		// Save list of class types according to it's sub-folder's designated obstacle type.
		if (filterPath.ToString().Contains(TEXT("AllTerrain")))
		{
			UE_LOG(LogLevelGen, Warning, TEXT("Added blueprints to descriptor group: %u"), static_cast<uint8>(EObstacleType::OT_ALL_TERRAIN_OBSTACLE));
			//_obstaclesTypes.Add(static_cast<uint8>(EObstacleType::OT_ALL_TERRAIN_OBSTACLE), obstacleClassTypes);
			_obstaclesTypes.Add(EObstacleType::OT_ALL_TERRAIN_OBSTACLE, obstacleClassTypes);
		}
		else if (filterPath.ToString().Contains(TEXT("Road")))
		{
			UE_LOG(LogLevelGen, Warning, TEXT("Added blueprints to descriptor group: %u"), static_cast<uint8>(EObstacleType::OT_ROAD_OBSTACLE));
			//_obstaclesTypes.Add(static_cast<uint8>(EObstacleType::OT_ROAD_OBSTACLE), obstacleClassTypes);
			_obstaclesTypes.Add(EObstacleType::OT_ROAD_OBSTACLE, obstacleClassTypes);
		}
		else if (filterPath.ToString().Contains(TEXT("Grass")))
		{
			UE_LOG(LogLevelGen, Warning, TEXT("Added blueprints to descriptor group: %u"), static_cast<uint8>(EObstacleType::OT_GRASS_OBSTACLE));
			//_obstaclesTypes.Add(static_cast<uint8>(EObstacleType::OT_GRASS_OBSTACLE), obstacleClassTypes);
			_obstaclesTypes.Add(EObstacleType::OT_GRASS_OBSTACLE, obstacleClassTypes);
		}
	}
}
#endif
