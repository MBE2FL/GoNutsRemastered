// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenLeftState.h"
#include "LevelGenUpState.h"
#include "GameFramework/Character.h"
#include "IntersectionChunk.h"
#include "FreeRoamCharacter.h"

DEFINE_LOG_CATEGORY(LogLevelGenLeftState);

ULevelGenState* ULevelGenLeftState::updateState()
{
	if (_initialized)
	{
		// Transition to forward generation state.
		if (_levelGen->getMapOrientation() == EMapOrientations::MO_Up)
		{
			return ALevelGenerator::getLevelGenUpState();
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

void ULevelGenLeftState::transition(const ALevelChunk* intersection)
{
	const AIntersectionChunk* interChunk = Cast<AIntersectionChunk>(intersection);

	USceneComponent* leftConnector = interChunk->getLeftConnector();

	if (!IsValid(leftConnector))
	{
		UE_LOG(LogLevelGenLeftState, Warning, TEXT("Intersection chunk, %s, does not have a left connector!"), *interChunk->GetName());
		return;
	}

	ALevelChunk* chunk = getValidChunk();

	if (!IsValid(chunk))
	{
		UE_LOG(LogLevelGenLeftState, Warning, TEXT("Spawn transition chunk, %s, did not spawn!"), *interChunk->GetName());
		return;
	}

	// Set the position and rotation of the new chunk.
	FRotator rotation = FRotator(0.0f, -90.0f, 0.0f);
	chunk->SetActorRotation(rotation);
	FVector position = leftConnector->GetComponentLocation();
	chunk->SetActorLocation(position);

	_prevChunk = chunk;

	UE_LOG(LogLevelGenLeftState, Warning, TEXT("Transition chunk, %s, spawned!"), *chunk->GetName());




	//FVector min;
	//FVector max;
	//chunk->getMesh()->GetLocalBounds(min, max);
	FVector boxSize = chunk->getMesh()->GetStaticMesh()->GetBounds().BoxExtent * 2.0f;
	//UE_LOG(LogLevelGenLeftState, Error, TEXT("Chunk, %s, Bounds: min %s, max %s!"), *chunk->GetName(), *min.ToString(), *max.ToString());
	UE_LOG(LogLevelGenLeftState, Error, TEXT("Chunk, %s, Bounds: %s!"), *chunk->GetName(), *boxSize.ToString());

}

void ULevelGenLeftState::update()
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
			FRotator rotation = FRotator(0.0f, -90.0f, 0.0f);
			chunk->SetActorRotation(rotation);
			FVector position = _prevChunk->GetActorLocation();
			//position.Y += _prevChunk->getMesh()->Bounds.BoxExtent.X * 2.0f;
			position.Y -= _prevChunk->getMesh()->GetStaticMesh()->GetBounds().BoxExtent.X;
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

			UE_LOG(LogLevelGenLeftState, Warning, TEXT("Spawned chunk: %s"), *chunk->GetName());
		}
		else if (IsValid(chunk))
		{
			//FVector position = _prevChunk->GetActorLocation();
			//position.X += _prevChunk->getMesh()->Bounds.BoxExtent.X * 2.0f;
			//chunk->SetActorLocation(position);


			_prevChunk = chunk;

			UE_LOG(LogLevelGenLeftState, Warning, TEXT("Spawned chunk: %s"), *chunk->GetName());
		}
	}
}

ALevelChunk* ULevelGenLeftState::getValidChunk()
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
			UE_LOG(LogLevelGenLeftState, Warning, TEXT("Prev Chunk: Town_Three_Lanes_Island"));

			// Spawn a chunk from the same pool of objects the previous chunk belongs to.
			// Spawn a chunk not from the same pool of objects the previous chunk belongs to, but is still valid.
			int32 testRand = FMath::RandRange(0, 100);

			if (testRand <= 90)
			{
				nextChunkDescriptor = prevChunkDescriptor;
			}
			else if (testRand > 90 && testRand <= 95)
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
			UE_LOG(LogLevelGenLeftState, Warning, TEXT("Prev Chunk: Town_Intersection_Three_Lanes"));

			// Spawn a chunk not from the same pool of objects the previous chunk belongs to, but is still valid.
			nextChunkDescriptor = ALevelChunk::TOWN_THREE_LANES_ISLAND;

		}
		else if ((prevChunkDescriptor | ALevelChunk::TOWN_THREE_TO_TWO_LANES_MERGER) == ALevelChunk::TOWN_THREE_TO_TWO_LANES_MERGER)
		{
			UE_LOG(LogLevelGenLeftState, Warning, TEXT("Prev Chunk: Town_Three_To_Two_Lanes_Merger"));

			// Spawn a chunk not from the same pool of objects the previous chunk belongs to, but is still valid.
			nextChunkDescriptor = ALevelChunk::TOWN_TWO_LANES;
		}
		else if ((prevChunkDescriptor | ALevelChunk::TOWN_TWO_LANES) == ALevelChunk::TOWN_TWO_LANES)
		{
			UE_LOG(LogLevelGenLeftState, Warning, TEXT("Prev Chunk: Town_Two_Lanes"));

			// Spawn a chunk not from the same pool of objects the previous chunk belongs to, but is still valid.
			nextChunkDescriptor = ALevelChunk::TOWN_TWO_LANES;
		}
	}
	// Spawn a default starter chunk.
	else
	{
		UE_LOG(LogLevelGenLeftState, Warning, TEXT("Prev Chunk: None"));
	}




	const FChunkClassTypes* chunkClassTypes = _levelGen->getChunkClassTypes().Find(static_cast<int32>(nextChunkDescriptor));
	if (!chunkClassTypes)
	{
		UE_LOG(LogLevelGenLeftState, Error, TEXT("Level Gen did not have any class types assigned to a chunk descriptor type: %d!"), static_cast<int32>(nextChunkDescriptor));
		return chunk;
	}
	const TArray<TSubclassOf<ALevelChunk>> classTypesArr = chunkClassTypes->_chunkClassTypes;

	if (classTypesArr.Num() <= 0)
	{
		UE_LOG(LogLevelGenLeftState, Error, TEXT("Level Gen did not have any class types assigned to a chunk descriptor type!"));
		return chunk;
	}

	TSubclassOf<ALevelChunk> classType = classTypesArr[FMath::RandRange(0, classTypesArr.Num() - 1)];
	//chunk = Cast<ALevelChunk>(_levelGen->GetWorld()->SpawnActor(classType));
	chunk = _levelGen->spawnChunk(classType);

	return chunk;
}
