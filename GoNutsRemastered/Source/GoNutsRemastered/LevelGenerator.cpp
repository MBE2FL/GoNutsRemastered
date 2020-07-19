// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
//#include "Engine/LevelStreaming.h"
//#include "EngineUtils.h"
//#include "LevelSegment.h"
#include "LevelGenUpState.h"
#include "LevelGenLeftState.h"

#include "AssetRegistryModule.h"

#include "ChunkObjectPool.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY(LogLevelGen);


ULevelGenUpState* ALevelGenerator::_levelGenUpState = NewObject<ULevelGenUpState>();
ULevelGenLeftState* ALevelGenerator::_levelGenLeftState = NewObject<ULevelGenLeftState>();


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

ALevelChunk* ALevelGenerator::spawnChunk(const TSubclassOf<ALevelChunk>& chunkClassType)
{
	return _chunkObjectPool->getLevelChunk(chunkClassType);
}

void ALevelGenerator::recycleChunk(ALevelChunk* chunk)
{
	_chunkObjectPool->recycleLevelChunk(chunk);
}

ACharacter* ALevelGenerator::getPlayer()
{
	if (!IsValid(_player))
	{
		_player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}

	return _player;
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();


	

	// Get a reference to the player.
	_player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Create the default generation state.
	_levelGenState = NewObject<ULevelGenUpState>();
	_levelGenState->init(this);

	// Create generation timer.
	GetWorldTimerManager().SetTimer(_timerHandle, this, &ALevelGenerator::updateLevelGen, 0.5f, true, 1.0f);

	// Create chunk memory pool.
	_chunkObjectPool = UChunkObjectPool::getInstance();
	_chunkObjectPool->init(this);
}

void ALevelGenerator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clear the generation timer.
	GetWorldTimerManager().ClearTimer(_timerHandle);

	// Cleanup states.
	//if (IsValid(_levelGenUpState))
	//{
	//	_levelGenUpState->cleanupState();
	//}

	//if (IsValid(_levelGenLeftState))
	//{
	//	_levelGenLeftState->cleanupState();
	//}

	// Cleanup the chunk memory pool.
	UChunkObjectPool::destroyInstance();
	GetWorld()->ForceGarbageCollection(true);
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

void ALevelGenerator::getAllChunkClassTypes()
{
#if WITH_EDITOR
	// Load the asset registry module
	FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& assetRegistry = assetRegistryModule.Get();

	TArray<FString> contentPaths;
	contentPaths.Add(TEXT("/Game/LevelChunks"));
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


	// Filter each sub-folder in the root LevelChunks folder.
	FARFilter filter;
	filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
	filter.bRecursiveClasses = true;
	filter.PackagePaths.Add(TEXT("/Game/LevelChunks/Town_3Lanes"));
	filter.bRecursivePaths = true;



	// Load all chunks, and store them according to their respective folder's designated chunk descriptor.
	for (const FName& path : filter.PackagePaths)
	{
		// Load all chunks in the current sub-folder.
		UE_LOG(LogLevelGen, Warning, TEXT("Searching for all level chunk types in %s."), *path.ToString());

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
		if (path.ToString().Contains(TEXT("Town_3Lanes")))
		{
			UE_LOG(LogLevelGen, Warning, TEXT("Added blueprints to descriptor group: %u"), static_cast<uint8>(ALevelChunk::TOWN_THREE_LANES_ISLAND));
			_chunks.Add(ALevelChunk::TOWN_THREE_LANES_ISLAND, chunkClassTypes);
		}
	}
#endif
}
