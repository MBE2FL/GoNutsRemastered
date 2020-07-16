// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenUpState.h"

#include "DrawDebugHelpers.h"
#include "LevelGenLeftState.h"

DEFINE_LOG_CATEGORY(LogLevelGenUpState);

ULevelGenState* ULevelGenUpState::updateState()
{
	// Transition to right generation state.
	if (_levelGen->getMapOrientation() == EMapOrientations::MO_Right)
	{
		return nullptr;
	}
	// Transition to left generation state.
	else if (_levelGen->getMapOrientation() == EMapOrientations::MO_Left)
	{
		return _levelGen->getLevelGenLeftState();
		//return ALevelGenerator::getLevelGenLeftState();
	}
	// Stay in current generation state.
	else
	{
		return nullptr;
	}
}

void ULevelGenUpState::update()
{
	ALevelChunk* chunk = nullptr;
	chunk = getValidChunk();

	//if (chunk)
	//{
	//	_prevChunk = chunk;
	//}

	if (IsValid(_prevChunk) && IsValid(chunk))
	{
		FVector position = _prevChunk->GetActorLocation();
		position.X += _prevChunk->getMesh()->Bounds.BoxExtent.X * 2.0f;
		//position.X += 2000.0f;
		chunk->SetActorLocation(position);

		
		// Notify listeners for chunk features.
		// TO-DO Make combined enum values.
		switch (chunk->getChunckFeatures())
		{
		case EChunkFeatures::CF_NONE:
			break;
		case EChunkFeatures::CF_NO_FEATURES:
			break;
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
	}
	else if (IsValid(chunk))
	{
		//FVector position = _prevChunk->GetActorLocation();
		//position.X += _prevChunk->getMesh()->Bounds.BoxExtent.X * 2.0f;
		//chunk->SetActorLocation(position);


		_prevChunk = chunk;
	}
}

ALevelChunk* ULevelGenUpState::getValidChunk()
{
	ALevelChunk* chunk = nullptr;

	// Spawn a chunk based on the previous chunk.
	//if (_prevChunk)
	if (IsValid(_prevChunk))
	{
		const EChunkDescriptors::Type prevChunkDescriptor = _prevChunk->getChunkDescriptors();

		if (prevChunkDescriptor & ALevelChunk::TOWN_THREE_LANES_ISLAND)
		{
			UE_LOG(LogLevelGenUpState, Warning, TEXT("Prev Chunk: Town_Three_Lanes_Island"));

			const FChunkClassTypes* chunkClassTypes = _levelGen->getChunkClassTypes().Find(static_cast<int32>(prevChunkDescriptor));
			if (!chunkClassTypes)
			{
				UE_LOG(LogLevelGen, Error, TEXT("Level Gen did not have any class types assigned to a chunk descriptor type: %d!"), static_cast<int32>(prevChunkDescriptor));
				return chunk;
			}
			const TArray<TSubclassOf<ALevelChunk>> classTypesArr = chunkClassTypes->_chunkClassTypes;

			if (classTypesArr.Num() <= 0)
			{
				UE_LOG(LogLevelGen, Error, TEXT("Level Gen did not have any class types assigned to a chunk descriptor type!"));
				return chunk;
			}

			TSubclassOf<ALevelChunk> classType = classTypesArr[FMath::RandRange(0, classTypesArr.Num() - 1)];
			chunk = Cast<ALevelChunk>(_levelGen->GetWorld()->SpawnActor(classType));

			return chunk;
		}
	}
	// Spawn a default starter chunk.
	else
	{
		UE_LOG(LogLevelGenUpState, Warning, TEXT("Prev Chunk: None"));

		const FChunkClassTypes* chunkClassTypes = _levelGen->getChunkClassTypes().Find(static_cast<int32>(ALevelChunk::TOWN_THREE_LANES_ISLAND));
		if (!chunkClassTypes)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Level Gen did not have any class types assigned to a chunk descriptor type: %d!"), static_cast<int32>(ALevelChunk::TOWN_THREE_LANES_ISLAND));
			return chunk;
		}
		const TArray<TSubclassOf<ALevelChunk>> classTypesArr = chunkClassTypes->_chunkClassTypes;

		if (classTypesArr.Num() <= 0)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Level Gen did not have any class types assigned to a chunk descriptor type!"));
			return chunk;
		}

		TSubclassOf<ALevelChunk> classType = classTypesArr[FMath::RandRange(0, classTypesArr.Num() - 1)];
		chunk = Cast<ALevelChunk>(_levelGen->GetWorld()->SpawnActor(classType));

		return chunk;
	}


	return chunk;
}
