// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenLeftState.h"
#include "LevelGenerator.h"
#include "LevelGenUpState.h"
#include "LevelGenDownState.h"
#include "IntersectionChunk.h"
#include "FreeRoamCharacter.h"

DEFINE_LOG_CATEGORY(LogLevelGenLeftState);

ULevelGenState* ULevelGenLeftState::updateState()
{
	if (_initialized)
	{
		// Transition to up generation state.
		if (_levelGen->getMapOrientation() == EMapOrientations::MO_Up)
		{
			return ALevelGenerator::getLevelGenUpState();
		}
		// Transition to down generation state.
		else if (_levelGen->getMapOrientation() == EMapOrientations::MO_Down)
		{
			return ALevelGenerator::getLevelGenDownState();
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

void ULevelGenLeftState::transition(const ALevelChunk* intersection, const EMapOrientations& prevMapOrientation)
{
	const AIntersectionChunk* interChunk = Cast<AIntersectionChunk>(intersection);

	USceneComponent* connector = nullptr;

	switch (prevMapOrientation)
	{
	case EMapOrientations::MO_Up:
		connector = interChunk->getLeftConnector();
		break;
	case EMapOrientations::MO_Down:
		connector = interChunk->getRightConnector();
		break;
	default:
		UE_LOG(LogLevelGenLeftState, Warning, TEXT("Intersection chunk, %s, does not have a valid connector!"), *interChunk->GetName());
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
	FVector position = connector->GetComponentLocation();
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
			position.Y -= _prevChunk->getMesh()->GetStaticMesh()->GetBounds().BoxExtent.X * 2.0f;
			chunk->SetActorLocation(position);


			// Notify listeners for chunk features.
			// TO-DO Make combined enum values.
			switch (chunk->getChunckFeatures())
			{
			case EChunkFeatures::CF_SPAWN_CARS:
				break;
			case EChunkFeatures::CF_SPAWN_OBSTACLES:
				_levelGen->onRoadSpawned().Broadcast(chunk);
				break;
			case EChunkFeatures::CF_SPAWN_PEDESTRIANS:
				_levelGen->onCrosswalkSpawned().Broadcast(chunk);
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
