// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenUpState.h"
#include "LevelGenerator.h"
#include "LevelGenLeftState.h"
#include "LevelGenRightState.h"
#include "IntersectionChunk.h"
#include "CustomCharacter.h"
//#if WITH_EDITOR
//#include "DrawDebugHelpers.h"
//#endif

DEFINE_LOG_CATEGORY(LogLevelGenUpState);

ULevelGenState* ULevelGenUpState::updateState()
{
	if (_initialized)
	{
		// Transition to right generation state.
		if (_levelGen->getMapOrientation() == EMapOrientations::MO_Right)
		{
			return ALevelGenerator::getLevelGenRightState();
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

void ULevelGenUpState::transition(const ALevelChunk* intersection, const EMapOrientations& prevMapOrientation)
{
	const AIntersectionChunk* interChunk = Cast<AIntersectionChunk>(intersection);

	USceneComponent* connector = nullptr;

	switch (prevMapOrientation)
	{
	case EMapOrientations::MO_Left:
		connector = interChunk->getRightConnector();
		break;
	case EMapOrientations::MO_Right:
		connector = interChunk->getLeftConnector();
		break;
	default:
		UE_LOG(LogLevelGenUpState, Warning, TEXT("Intersection chunk, %s, does not have a valid connector!"), *interChunk->GetName());
		return;
	}

	//if (!IsValid(connector))
	//{
	//	UE_LOG(LogLevelGenUpState, Warning, TEXT("Intersection chunk, %s, does not have an up connector!"), *interChunk->GetName());
	//	return;
	//}

	ALevelChunk* chunk = getValidChunk();

	if (!IsValid(chunk))
	{
		UE_LOG(LogLevelGenUpState, Warning, TEXT("Spawn transition chunk, %s, did not spawn!"), *interChunk->GetName());
		return;
	}

	// Set the position and rotation of the new chunk.
	FRotator rotation = FRotator(0.0f, 0.0f, 0.0f);
	chunk->SetActorRotation(rotation);
	FVector position = connector->GetComponentLocation();
	chunk->SetActorLocation(position);

	_prevChunk = chunk;

	UE_LOG(LogLevelGenUpState, Warning, TEXT("Transition chunk, %s, spawned!"), *chunk->GetName());
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
				_levelGen->onRoadSpawned().Broadcast(chunk);
				break;
			case EChunkFeatures::CF_SPAWN_PEDESTRIANS:
				_levelGen->onCrosswalkSpawned().Broadcast(chunk);
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
			chunk->SetActorLocation(FVector::ZeroVector);

			UE_LOG(LogLevelGenUpState, Warning, TEXT("Spawned chunk: %s"), *chunk->GetName());
		}
	}
}
