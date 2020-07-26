// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenUpState.h"

#include "DrawDebugHelpers.h"
#include "LevelGenLeftState.h"

//#include "GameFramework/Character.h"
#include "FreeRoamCharacter.h"

DEFINE_LOG_CATEGORY(LogLevelGenUpState);

ULevelGenState* ULevelGenUpState::updateState()
{
	if (_initialized)
	{
		// Transition to right generation state.
		if (_levelGen->getMapOrientation() == EMapOrientations::MO_Right)
		{
			return nullptr;
		}
		// Transition to left generation state.
		else if (_levelGen->getMapOrientation() == EMapOrientations::MO_Left)
		{
			//return _levelGen->getLevelGenLeftState();
			return ALevelGenerator::getLevelGenLeftState();
		}
		// Stay in current generation state.
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
}

void ULevelGenUpState::update()
{
	if (_initialized)
	{
		// Check if the player is still in the game.
		if (!IsValid(_player))
		{
			_player = _levelGen->getPlayer();
			return;
		}


		// Check whether to keep spawning chunks ahead of the player.
		if (IsValid(_prevChunk))
		{
			if (FVector::DistSquaredXY(_player->GetActorLocation(), _prevChunk->GetActorLocation()) >= FMath::Square(8000.0f))
			{
				return;
			}
		}

		// Attempt to spawn a chunk.
		ALevelChunk* chunk = nullptr;
		chunk = getValidChunk();


		if (IsValid(_prevChunk) && IsValid(chunk))
		{
			// Set the position and rotation of the new chunk.
			FRotator rotation = FRotator(0.0f, 0.0f, 0.0f);
			chunk->SetActorRotation(rotation);
			FVector position = _prevChunk->GetActorLocation();
			position.X += _prevChunk->getMesh()->Bounds.BoxExtent.X * 2.0f;
			chunk->SetActorLocation(position);


			// Notify listeners for chunk features.
			// TO-DO Make combined enum values.
			switch (chunk->getChunckFeatures())
			{
			case EChunkFeatures::CF_SPAWN_CARS:
				break;
			case EChunkFeatures::CF_SPAWN_OBSTACLES:
				_levelGen->onRoadSpawned().Broadcast(chunk, chunk->getObstacleSpawnPoints());
				break;
			case EChunkFeatures::CF_SPAWN_PEDESTRIANS:
				_levelGen->onCrosswalkSpawned().Broadcast(chunk, chunk->getPedestrianSpawnPoints());
				break;
			default:
				break;
			}


			_prevChunk = chunk;

			UE_LOG(LogLevelGenUpState, Warning, TEXT("Spawned chunk: %s"), *chunk->GetName());
		}
		else if (IsValid(chunk))
		{
			//FVector position = _prevChunk->GetActorLocation();
			//position.X += _prevChunk->getMesh()->Bounds.BoxExtent.X * 2.0f;
			//chunk->SetActorLocation(position);


			_prevChunk = chunk;

			UE_LOG(LogLevelGenUpState, Warning, TEXT("Spawned chunk: %s"), *chunk->GetName());
		}
	}
}

ALevelChunk* ULevelGenUpState::getValidChunk()
{
	ALevelChunk* chunk = nullptr;
	EChunkDescriptors::Type nextChunkDescriptor = ALevelChunk::TOWN_THREE_LANES_ISLAND;

	// Spawn a chunk based on the previous chunk.
	//if (_prevChunk)
	if (IsValid(_prevChunk))
	{
		const EChunkDescriptors::Type prevChunkDescriptor = _prevChunk->getChunkDescriptors();

		// Previous chunk(Biome: Town, Lanes: 3)
		if ((prevChunkDescriptor | ALevelChunk::TOWN_THREE_LANES_ISLAND) == ALevelChunk::TOWN_THREE_LANES_ISLAND)
		{
			UE_LOG(LogLevelGenUpState, Warning, TEXT("Prev Chunk: Town_Three_Lanes_Island"));

			// Spawn a chunk from the same pool of objects the previous chunk belongs to.
			// Spawn a chunk not from the same pool of objects the previous chunk belongs to, but is still valid.
			int32 testRand = FMath::RandRange(0, 100);

			if (testRand <= 60)
			{
				nextChunkDescriptor = prevChunkDescriptor;
			}
			else if (testRand > 60 && testRand <= 95)
			{
				nextChunkDescriptor = ALevelChunk::TOWN_THREE_LANES_INTERSECTION;
			}
			else if (testRand > 95)
			{
				nextChunkDescriptor = ALevelChunk::TOWN_THREE_TO_TWO_LANES_MERGER;
			}

		}
		else if ((prevChunkDescriptor | ALevelChunk::TOWN_THREE_LANES_INTERSECTION) == ALevelChunk::TOWN_THREE_LANES_INTERSECTION)
		{
			UE_LOG(LogLevelGenUpState, Warning, TEXT("Prev Chunk: Town_Intersection_Three_Lanes"));

			// Spawn a chunk not from the same pool of objects the previous chunk belongs to, but is still valid.
			nextChunkDescriptor = ALevelChunk::TOWN_THREE_LANES_ISLAND;

		}
		else if ((prevChunkDescriptor | ALevelChunk::TOWN_THREE_TO_TWO_LANES_MERGER) == ALevelChunk::TOWN_THREE_TO_TWO_LANES_MERGER)
		{
			UE_LOG(LogLevelGenUpState, Warning, TEXT("Prev Chunk: Town_Three_To_Two_Lanes_Merger"));

			// Spawn a chunk not from the same pool of objects the previous chunk belongs to, but is still valid.
			nextChunkDescriptor = ALevelChunk::TOWN_TWO_LANES;
		}
		else if ((prevChunkDescriptor | ALevelChunk::TOWN_TWO_LANES) == ALevelChunk::TOWN_TWO_LANES)
		{
			UE_LOG(LogLevelGenUpState, Warning, TEXT("Prev Chunk: Town_Two_Lanes"));

			// Spawn a chunk not from the same pool of objects the previous chunk belongs to, but is still valid.
			nextChunkDescriptor = ALevelChunk::TOWN_TWO_LANES;
		}
	}
	// Spawn a default starter chunk.
	else
	{
		UE_LOG(LogLevelGenUpState, Warning, TEXT("Prev Chunk: None"));
	}




	const FChunkClassTypes* chunkClassTypes = _levelGen->getChunkClassTypes().Find(static_cast<int32>(nextChunkDescriptor));
	if (!chunkClassTypes)
	{
		UE_LOG(LogLevelGenUpState, Error, TEXT("Level Gen did not have any class types assigned to a chunk descriptor type: %d!"), static_cast<int32>(nextChunkDescriptor));
		return chunk;
	}
	const TArray<TSubclassOf<ALevelChunk>> classTypesArr = chunkClassTypes->_chunkClassTypes;

	if (classTypesArr.Num() <= 0)
	{
		UE_LOG(LogLevelGenUpState, Error, TEXT("Level Gen did not have any class types assigned to a chunk descriptor type!"));
		return chunk;
	}

	TSubclassOf<ALevelChunk> classType = classTypesArr[FMath::RandRange(0, classTypesArr.Num() - 1)];
	//chunk = Cast<ALevelChunk>(_levelGen->GetWorld()->SpawnActor(classType));
	chunk = _levelGen->spawnChunk(classType);

	return chunk;
}
