// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenState.h"
#include "LevelGenerator.h"
#include "LevelChunk.h"
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

ALevelChunk* ULevelGenState::getValidChunk()
{
	ALevelChunk* chunk = nullptr;
	EChunkDescriptors nextChunkDescriptor = EChunkDescriptors::CD_TOWN_THREE_LANES_ISLAND;

	// Spawn a chunk based on the previous chunk.
	//if (_prevChunk)
	if (IsValid(_prevChunk))
	{
		const EChunkDescriptors prevChunkDescriptor = _prevChunk->getChunkDescriptor();

		switch (prevChunkDescriptor)
		{
		// Previous chunk(Biome: Town, Lanes: 3)
		case EChunkDescriptors::CD_TOWN_THREE_LANES_ISLAND:
		{
			UE_LOG(LogLevelGenState, Warning, TEXT("Prev Chunk: Town_Three_Lanes_Island"));

			// Spawn a chunk from the same pool of objects the previous chunk belongs to.
			// Spawn a chunk not from the same pool of objects the previous chunk belongs to, but is still valid.
			int32 testRand = FMath::RandRange(0, 100);

			if (testRand <= 60)
			{
				nextChunkDescriptor = prevChunkDescriptor;
			}
			else if (testRand > 60 && testRand <= 99)
			{
				nextChunkDescriptor = EChunkDescriptors::CD_TOWN_THREE_LANES_INTERSECTION;
			}
			else if (testRand > 99)
			{
				nextChunkDescriptor = EChunkDescriptors::CD_TOWN_THREE_TO_TWO_LANES_MERGER;
			}
		}
			break;
		case EChunkDescriptors::CD_TOWN_THREE_LANES_INTERSECTION:
			UE_LOG(LogLevelGenState, Warning, TEXT("Prev Chunk: Town_Intersection_Three_Lanes"));

			// Spawn a chunk not from the same pool of objects the previous chunk belongs to, but is still valid.
			nextChunkDescriptor = EChunkDescriptors::CD_TOWN_THREE_LANES_ISLAND;
			break;
		case EChunkDescriptors::CD_TOWN_TWO_LANES:
			UE_LOG(LogLevelGenState, Warning, TEXT("Prev Chunk: Town_Two_Lanes"));

			// Spawn a chunk not from the same pool of objects the previous chunk belongs to, but is still valid.
			nextChunkDescriptor = EChunkDescriptors::CD_TOWN_TWO_LANES;
			break;
		case EChunkDescriptors::CD_TOWN_THREE_TO_TWO_LANES_MERGER:
			UE_LOG(LogLevelGenState, Warning, TEXT("Prev Chunk: Town_Three_To_Two_Lanes_Merger"));

			// Spawn a chunk not from the same pool of objects the previous chunk belongs to, but is still valid.
			nextChunkDescriptor = EChunkDescriptors::CD_TOWN_TWO_LANES;
			break;
		default:
			UE_LOG(LogLevelGenState, Warning, TEXT("Prev Chunk: None"));
			break;
		}
	}
	// Spawn a default starter chunk.
	else
	{
		UE_LOG(LogLevelGenState, Warning, TEXT("Prev Chunk: None"));
	}




	const FChunkClassTypes* chunkClassTypes = _levelGen->getChunkClassTypes().Find(nextChunkDescriptor);
	if (!chunkClassTypes)
	{
		UE_LOG(LogLevelGenState, Error, TEXT("Level Gen did not have any class types assigned to a chunk descriptor type: %d!"), static_cast<uint8>(nextChunkDescriptor));
		return chunk;
	}
	const TArray<TSubclassOf<ALevelChunk>> classTypesArr = chunkClassTypes->_chunkClassTypes;

	if (classTypesArr.Num() <= 0)
	{
		UE_LOG(LogLevelGenState, Error, TEXT("Level Gen did not have any class types assigned to a chunk descriptor type!"));
		return chunk;
	}

	TSubclassOf<ALevelChunk> classType = classTypesArr[FMath::RandRange(0, classTypesArr.Num() - 1)];
	//chunk = Cast<ALevelChunk>(_levelGen->GetWorld()->SpawnActor(classType));
	chunk = _levelGen->spawnChunk(classType);

	return chunk;
}
