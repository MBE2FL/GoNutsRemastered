// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAltLaneMoveState.h"
#include "PlayerDefaultMoveState.h"
#include "LevelChunk.h"
#include "LaneCharacter.h"
#include "LaneComponent.h"
#include "LevelGenerator.h"
#include "IntersectionChunk.h"

DEFINE_LOG_CATEGORY(LogPlayerAltLaneMoveState);

UPlayerAltLaneMoveState::UPlayerAltLaneMoveState()
{
}

UPlayerAltLaneMoveState::~UPlayerAltLaneMoveState()
{
}

UPlayerMoveState* UPlayerAltLaneMoveState::updateState()
{
	switch (_transitionState)
	{
	case ETransitionState::TS_DEFAULT:
	{
		_transitionDirection = ETransitionDirection::TD_NONE;
		_transitionState = ETransitionState::TS_NONE;
		UPlayerDefaultMoveState* moveState = ALaneCharacter::getPlayerDefaultMoveState();
		moveState->stateTransition(this);
		UE_LOG(LogPlayerAltLaneMoveState, Log, TEXT("STATE changed from alt lane to default!"));
		return moveState;
	}
	default:
		return nullptr;
	}
}

void UPlayerAltLaneMoveState::transition()
{
}

void UPlayerAltLaneMoveState::update(float deltaTime)
{
	if (!_initialized)
	{
		return;
	}


	// Move this character forward.
	_player->AddMovementInput(_player->GetActorForwardVector(), 0.5f);



	// TO-DO Check if lane has ended. For example a merger lane.
	// Smoothly move to a new lane.
	if (_moveToNewLane && IsValid(_currentLane))
	{
		// Convert this player's world position to the local position of the current chunk underneath this player.
		FVector playerPos = _player->GetActorLocation();
		FVector relativePlayerPos = _player->_chunk->GetActorTransform().InverseTransformPosition(playerPos);

		// Adjust the local X position and convert back to world space. Set this player's new position.
		relativePlayerPos.X = FMath::FInterpTo(relativePlayerPos.X, _targetY, deltaTime, _laneSwitchSpeed);

		playerPos = _player->_chunk->GetActorTransform().TransformPosition(relativePlayerPos);
		_player->SetActorLocation(playerPos);

		// Set the player's position to the target position, once it has moved close enough.
		if (FMath::Abs(relativePlayerPos.X - _targetY) <= 10.0f)
		{
			_moveToNewLane = false;
			_player->SetActorLocation(playerPos);
			_laneSwitchDirection = ELaneSwitchDirection::LSD_NONE;
			UE_LOG(LogPlayerAltLaneMoveState, Log, TEXT("Finished Lane Switch"));
		}
	}
}

void UPlayerAltLaneMoveState::switchToLeftLane()
{
	if (IsValid(_player->_chunk) && IsValid(_currentLane))
	{
		ULaneComponent* leftLane = nullptr;
		switch (_transitionDirection)
		{
		case ETransitionDirection::TD_LEFT:
			leftLane = _currentLane->getLeftLane();
			break;
		case ETransitionDirection::TD_RIGHT:
			leftLane = _currentLane->getRightLane();
			break;
		default:
			break;
		}

		// Check if the current lane has a lane to the left of it.
		//ULaneComponent* leftLane = _currentLane->getLeftLane();
		if (IsValid(leftLane))
		{
			// Convert this player's world position to the local position of the current chunk underneath this player.
			FVector playerPos = _player->GetActorLocation();
			FVector relativePlayerPos = _player->_chunk->GetActorTransform().InverseTransformPosition(playerPos);

			// TO-DO Test this out
			// Make sure left lane encompasses the area next to this player.
			//if (FMath::Abs(relativePlayerPos.Y) >= (FMath::Abs(leftLane->GetRelativeLocation().Y) + leftLane->GetUnscaledBoxExtent().Y))
			//{
			//	UE_LOG(LogPlayerAltLaneMoveState, Log, TEXT("Failed to switch to left lane!: Lane does not encompass the area next to the player!"));
			//	return;
			//}


			_targetY = leftLane->GetRelativeLocation().X;
			_moveToNewLane = true;


			// Update current lane.
			UE_LOG(LogPlayerAltLaneMoveState, Log, TEXT("Lane changing from current: %s to target: %s"), *_currentLane->GetName(), *leftLane->GetName());
			_currentLane = leftLane;
			_laneSwitchDirection = ELaneSwitchDirection::LSD_LEFT;
		}
	}
}

void UPlayerAltLaneMoveState::switchToRightLane()
{
	if (IsValid(_player->_chunk) && IsValid(_currentLane))
	{
		ULaneComponent* rightLane = nullptr;
		switch (_transitionDirection)
		{
		case ETransitionDirection::TD_LEFT:
			rightLane = _currentLane->getRightLane();
			break;
		case ETransitionDirection::TD_RIGHT:
			rightLane = _currentLane->getLeftLane();
			break;
		default:
			break;
		}


		// Check if the current lane has a lane to the right of it.
		//ULaneComponent* rightLane = _currentLane->getRightLane();
		if (IsValid(rightLane))
		{
			// Convert this player's world position to the local position of the current chunk underneath this player.
			FVector playerPos = _player->GetActorLocation();
			FVector relativePlayerPos = _player->_chunk->GetActorTransform().InverseTransformPosition(playerPos);

			// Make sure left lane encompasses the area next to this player.
			//if (FMath::Abs(relativePlayerPos.Y) >= (FMath::Abs(rightLane->GetRelativeLocation().Y) + rightLane->GetUnscaledBoxExtent().Y))
			//{
			//	UE_LOG(LogPlayerAltLaneMoveState, Log, TEXT("Failed to switch to right lane!: Lane does not encompass the area next to the player!"));
			//	return;
			//}


			_targetY = rightLane->GetRelativeLocation().X;
			_moveToNewLane = true;


			// Update current lane.
			UE_LOG(LogPlayerAltLaneMoveState, Log, TEXT("Lane changing from current: %s to target: %s"), *_currentLane->GetName(), *rightLane->GetName());
			_currentLane = rightLane;
			_laneSwitchDirection = ELaneSwitchDirection::LSD_RIGHT;
		}
	}
}

void UPlayerAltLaneMoveState::onBeginOverlap(AActor* otherActor, UPrimitiveComponent* otherComp)
{
	//UE_LOG(LogLaneCharacter, Error, TEXT("Overlapped Chunk: %s, Comp: %s"), *OtherActor->GetName(), *OtherComp->GetName());

	switch (otherComp->GetCollisionObjectType())
	{
		// Player enters a new lane, and did not come from a previous lane.
	case ECC_LaneChannel:
		if (IsValid(otherComp))
		{
			_lastOverlappedLane = Cast<ULaneComponent>(otherComp);

			// Iff no lane is set, immediately set current lane to the last overlapped lane.
			if (!IsValid(_currentLane))
			{
				_currentLane = _lastOverlappedLane;
			}
		}
		break;
		// Player enters a new level chunk.
	case ECC_LevelChunkChannel:
		if (IsValid(otherActor))
		{
			// Ignore iff the player is still on the intersection chunk.
			if (_player->_chunk == otherActor)
				return;

			_player->_chunk = Cast<ALevelChunk>(otherActor);

			UE_LOG(LogPlayerAltLaneMoveState, Log, TEXT("Chunk underneath changed to %s."), *_player->_chunk->GetName());

			_transitionState = ETransitionState::TS_DEFAULT;

			// Player is in the process of moving to a new lane.
			if (_moveToNewLane)
			{
				float _currentLaneY = _player->_chunk->GetActorTransform().InverseTransformPosition(_currentLane->GetComponentLocation()).Y;

				for (ULaneComponent* lane : _player->_chunk->getLanes())
				{
					if (FMath::Abs(lane->GetRelativeLocation().Y - _currentLaneY) < 100.0f)
					{
						_currentLane = lane;
						_targetY = lane->GetRelativeLocation().Y;
						UE_LOG(LogPlayerAltLaneMoveState, Log, TEXT("Current Lane switched to: %s"), *_currentLane->GetName());
					}
				}
			}
			// Player is not in the process of moving to a new lane.
			// Can't use _lastOverlappedLane, as order of collisions is random and it may noy be up to date yet.
			else
			{
				float _currentLaneY = _player->_chunk->GetActorTransform().InverseTransformPosition(_currentLane->GetComponentLocation()).Y;

				for (ULaneComponent* lane : _player->_chunk->getLanes())
				{
					if (FMath::Abs(lane->GetRelativeLocation().Y - _currentLaneY) < 100.0f)
					{
						_currentLane = lane;
						UE_LOG(LogPlayerAltLaneMoveState, Log, TEXT("Current Lane switched to: %s"), *_currentLane->GetName());
					}
				}
			}
		}
		break;
	default:
		break;
	}
}
