// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
//#include "Engine/LevelStreaming.h"
//#include "EngineUtils.h"
#include "LevelChunk.h"
#include "LevelGenUpState.h"
#include "LevelGenLeftState.h"
#include "LevelGenRightState.h"
#include "LevelGenDownState.h"

#include "AssetRegistryModule.h"

#include "ChunkObjectPool.h"
#include "CustomCharacter.h"
#include "ObstaclePool.h"

DEFINE_LOG_CATEGORY(LogLevelGen);


ULevelGenUpState* ALevelGenerator::_levelGenUpState = nullptr;
//ULevelGenLeftState* ALevelGenerator::_levelGenLeftState = NewObject<ULevelGenLeftState>();
ULevelGenLeftState* ALevelGenerator::_levelGenLeftState = nullptr;
ULevelGenRightState* ALevelGenerator::_levelGenRightState = nullptr;
ULevelGenDownState* ALevelGenerator::_levelGenDownState = nullptr;


// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

//const TSet<FSegmentSpawnInfo>& ALevelGenerator::getValidRightSegments(const ESegmentTypes& segmentType) const
//{
//	return _validSegmentsLookup.Find(segmentType)->_validRightSegments;
//}

#if WITH_EDITOR
void ALevelGenerator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName propertyName = PropertyChangedEvent.GetPropertyName();

	if (propertyName == GET_MEMBER_NAME_CHECKED(ALevelGenerator, _refreshChunkClassTypes))
	{
		if (_refreshChunkClassTypes)
		{
			UE_LOG(LogLevelGen, Warning, TEXT("Refreshing chunk class types..."));
			getAllChunkClassTypes();
			_refreshChunkClassTypes = false;
		}
	}
}
#endif

void ALevelGenerator::setMapOrientation(const bool& turnLeft)
{
	// Save previous map orientation for state transitions.
	_prevMapOrientation = _mapOrientation;

	// Determine current map orientation.
	switch (_mapOrientation)
	{
	case EMapOrientations::MO_Right:
		if (turnLeft)
		{
			_mapOrientation = EMapOrientations::MO_Up;
		}
		else
		{
			_mapOrientation = EMapOrientations::MO_Down;
		}
		break;
	case EMapOrientations::MO_Left:
		if (turnLeft)
		{
			_mapOrientation = EMapOrientations::MO_Down;
		}
		else
		{
			_mapOrientation = EMapOrientations::MO_Up;
		}
		break;
	case EMapOrientations::MO_Up:
		if (turnLeft)
		{
			_mapOrientation = EMapOrientations::MO_Left;
		}
		else
		{
			_mapOrientation = EMapOrientations::MO_Right;
		}
		break;
	case EMapOrientations::MO_Down:
		if (turnLeft)
		{
			_mapOrientation = EMapOrientations::MO_Right;
		}
		else
		{
			_mapOrientation = EMapOrientations::MO_Left;
		}
		break;
	default:
		break;
	}
}

ALevelChunk* ALevelGenerator::spawnChunk(const TSubclassOf<ALevelChunk>& chunkClassType)
{
	return _chunkObjectPool->getLevelChunk(chunkClassType);
}

void ALevelGenerator::recycleChunk(ALevelChunk* chunk)
{
	_chunkObjectPool->recycleLevelChunk(chunk);
}

ACustomCharacter* ALevelGenerator::getPlayer()
{
	if (!IsValid(_player))
	{
		_player = Cast<ACustomCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (IsValid(_player))
		{
			_player->init(this);
		}
	}

	return _player;
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();


	

	// Get a reference to the player.
	//_player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	getPlayer();

	// Create the default generation state.
	_levelGenUpState = NewObject<ULevelGenUpState>();
	_levelGenLeftState = NewObject<ULevelGenLeftState>();
	_levelGenRightState = NewObject<ULevelGenRightState>();
	_levelGenDownState = NewObject<ULevelGenDownState>();
	_levelGenUpState->AddToRoot();
	_levelGenLeftState->AddToRoot();
	_levelGenRightState->AddToRoot();
	_levelGenDownState->AddToRoot();
	_levelGenUpState->init(this);
	_levelGenLeftState->init(this);
	_levelGenRightState->init(this);
	_levelGenDownState->init(this);
	_levelGenState = _levelGenUpState;

	// Create generation timer.
	//GetWorldTimerManager().SetTimer(_timerHandle, this, &ALevelGenerator::updateLevelGen, 0.5f, true, 1.0f);

	// Create chunk memory pool.
	_chunkObjectPool = UChunkObjectPool::getInstance();
	_chunkObjectPool->init(this);
}

void ALevelGenerator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clear the generation timer.
	//GetWorldTimerManager().ClearTimer(_timerHandle);


	// Cleanup states.
	if (IsValid(_levelGenUpState))
	{
		_levelGenUpState->cleanupState();
		_levelGenUpState = nullptr;
	}

	if (IsValid(_levelGenLeftState))
	{
		_levelGenLeftState->cleanupState();
		_levelGenLeftState = nullptr;
	}

	if (IsValid(_levelGenRightState))
	{
		_levelGenRightState->cleanupState();
		_levelGenRightState = nullptr;
	}

	if (IsValid(_levelGenDownState))
	{
		_levelGenDownState->cleanupState();
		_levelGenDownState = nullptr;
	}

	_levelGenState = nullptr;


	// Cleanup the chunk memory pool.
	UChunkObjectPool::destroyInstance();
	UObstaclePool::deleteInstance();
	//GetWorld()->ForceGarbageCollection(true);
	GEngine->ForceGarbageCollection(true);

	_chunkObjectPool = nullptr;
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	updateLevelGen();
}

void ALevelGenerator::updateLevelGen()
{
	ULevelGenState* currLevelGenState = _levelGenState->updateState();

	// State has changed.
	if (IsValid(currLevelGenState))
	{
		if (IsValid(_player))
		{
			_levelGenState = currLevelGenState;
			_levelGenState->transition(_player->getChunk(), _prevMapOrientation);
			_levelGenState->update();
		}
	}
	// Still in previous state.
	else
	{
		_levelGenState->update();
	}
}

#if WITH_EDITOR
void ALevelGenerator::getAllChunkClassTypes()
{
	// Load the asset registry module
	FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& assetRegistry = assetRegistryModule.Get();

	TArray<FString> contentPaths;
	contentPaths.Add(TEXT("/Game/LevelChunks/Chunks"));
	assetRegistry.ScanFilesSynchronous(contentPaths);

	FName baseClassName = ALevelChunk::StaticClass()->GetFName();

	// Get all derived class names.
	TSet<FName> derivedNames;
	{
		TArray<FName> baseNames;
		baseNames.Add(baseClassName);

		TSet<FName> excluded;
		assetRegistry.GetDerivedClassNames(baseNames, excluded, derivedNames);
	}


	TArray<FName> filterPaths;
	filterPaths.Reserve(4);
	filterPaths.Add(TEXT("/Game/LevelChunks/Chunks/Town_3Lanes"));
	filterPaths.Add(TEXT("/Game/LevelChunks/Chunks/Town_Intersection_3Lanes"));
	filterPaths.Add(TEXT("/Game/LevelChunks/Chunks/Town_2Lanes"));
	filterPaths.Add(TEXT("/Game/LevelChunks/Chunks/Town_3To2Lanes_Merger"));


	for (const FName& filterPath : filterPaths)
	{
		UE_LOG(LogLevelGen, Warning, TEXT("Searching for all level chunk types in %s."), *filterPath.ToString());

		// Filter each sub-folder in the root LevelChunks folder.
		FARFilter filter;
		filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
		filter.bRecursiveClasses = true;
		filter.PackagePaths.Add(filterPath);
		filter.bRecursivePaths = true;

		// Load all chunks, and store them according to their respective folder's designated chunk descriptor.
		TArray<FAssetData> assetList;
		assetRegistry.GetAssets(filter, assetList);

		FChunkClassTypes chunkClassTypes{};
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

				// Store chunk according to it's class type.
				TAssetSubclassOf<ALevelChunk> test = TAssetSubclassOf<ALevelChunk>(FStringAssetReference(classObjectPath));
				TSubclassOf<ALevelChunk> chunkClassType = test.Get();
				UE_LOG(LogLevelGen, Warning, TEXT("Found blueprint: %s"), *test.Get()->GetName());

				chunkClassTypes._chunkClassTypes.Add(chunkClassType);
			}
		}

		// Save list of class types according to it's sub-folder's designated chunk descriptor.
		if (filterPath.ToString().Contains(TEXT("Town_3Lanes")))
		{
			UE_LOG(LogLevelGen, Warning, TEXT("Added blueprints to descriptor group: %u"), EChunkDescriptors::CD_TOWN_THREE_LANES_ISLAND);
			_chunkClassTypes.Add(EChunkDescriptors::CD_TOWN_THREE_LANES_ISLAND, chunkClassTypes);
		}
		else if (filterPath.ToString().Contains(TEXT("Town_Intersection_3Lanes")))
		{
			UE_LOG(LogLevelGen, Warning, TEXT("Added blueprints to descriptor group: %u"), EChunkDescriptors::CD_TOWN_THREE_LANES_INTERSECTION);
			_chunkClassTypes.Add(EChunkDescriptors::CD_TOWN_THREE_LANES_INTERSECTION, chunkClassTypes);
		}
		else if (filterPath.ToString().Contains(TEXT("Town_2Lanes")))
		{
			UE_LOG(LogLevelGen, Warning, TEXT("Added blueprints to descriptor group: %u"), EChunkDescriptors::CD_TOWN_TWO_LANES);
			_chunkClassTypes.Add(EChunkDescriptors::CD_TOWN_TWO_LANES, chunkClassTypes);
		}
		else if (filterPath.ToString().Contains(TEXT("Town_3To2Lanes_Merger")))
		{
			UE_LOG(LogLevelGen, Warning, TEXT("Added blueprints to descriptor group: %u"), EChunkDescriptors::CD_TOWN_THREE_TO_TWO_LANES_MERGER);
			_chunkClassTypes.Add(EChunkDescriptors::CD_TOWN_THREE_TO_TWO_LANES_MERGER, chunkClassTypes);
		}
	}
}
#endif
