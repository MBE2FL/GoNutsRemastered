// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDefaultMoveState.h"
#include "PlayerTurningMoveState.h"
#include "LevelChunk.h"
#include "LaneCharacter.h"
#include "LaneComponent.h"
#include "LevelGenerator.h"
#include "IntersectionChunk.h"


DEFINE_LOG_CATEGORY(LogPlayerDefaultMoveState);

UPlayerDefaultMoveState::UPlayerDefaultMoveState()
{
}

UPlayerDefaultMoveState::~UPlayerDefaultMoveState()
{
}

UPlayerMoveState* UPlayerDefaultMoveState::updateState()
{
	switch (_transitionState)
	{
	case ETransitionState::TS_TURNING:
	{
		_transitionState = ETransitionState::TS_NONE;
		UPlayerTurningMoveState* moveState = ALaneCharacter::getPlayerTurningMoveState();
		moveState->stateTransition(this);
		UE_LOG(LogPlayerDefaultMoveState, Error, TEXT("STATE changed from default to turning!"));
		return moveState;
	}
	default:
		return nullptr;
	}
}

void UPlayerDefaultMoveState::update(float deltaTime)
{
	if (!_initialized)
	{
		return;
	}


	if (_chunkChanged)
	{
		_chunkChanged = false;

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
					UE_LOG(LogPlayerDefaultMoveState, Warning, TEXT("Current Lane switched to: %s"), *_currentLane->GetName());
				}
			}
		}
		// Player is not in the process of moving to a new lane.
		else
		{
			_currentLane = _lastOverlappedLane;
			UE_LOG(LogPlayerDefaultMoveState, Warning, TEXT("Current Lane switched to: %s"), *_currentLane->GetName());
		}
	}



	// Move this character forward.
	_player->AddMovementInput(_player->GetActorForwardVector(), 0.5f);


	// Zero out pitch and roll.
	FRotator rotator = FRotator(0.0f, _player->GetControlRotation().Yaw, 0.0f);
	//FRotator rotator = FRotator(0.0f, GetActorRotation().Yaw, 0.0f);
	//_player->SetActorRotation(rotator);


	// TO-DO Check if lane has ended. For example a merger lane.
	// Smoothly move to a new lane.
	if (_moveToNewLane && IsValid(_currentLane))
	{
		// Convert this player's world position to the local position of the current chunk underneath this player.
		FVector playerPos = _player->GetActorLocation();
		FVector relativePlayerPos = _player->_chunk->GetActorTransform().InverseTransformPosition(playerPos);

		// Adjust the local Y position and convert back to world space. Set this player's new position.
		relativePlayerPos.Y = FMath::FInterpTo(relativePlayerPos.Y, _targetY, deltaTime, _laneSwitchSpeed);

		playerPos = _player->_chunk->GetActorTransform().TransformPosition(relativePlayerPos);
		_player->SetActorLocation(playerPos);

		// Set the player's position to the target position, once it has moved close enough.
		if (FMath::Abs(relativePlayerPos.Y - _targetY) <= 10.0f)
		{
			_moveToNewLane = false;
			_player->SetActorLocation(playerPos);
			_laneSwitchDirection = ELaneSwitchDirection::LSD_NONE;
			UE_LOG(LogPlayerDefaultMoveState, Warning, TEXT("Finished Lane Switch"));
		}
	}
}

void UPlayerDefaultMoveState::switchToLeftLane()
{
	if (IsValid(_player->_chunk) && IsValid(_currentLane))
	{
		// Check if the current lane has a lane to the left of it.
		ULaneComponent* leftLane = _currentLane->getLeftLane();
		if (IsValid(leftLane))
		{
			// Convert this player's world position to the local position of the current chunk underneath this player.
			FVector playerPos = _player->GetActorLocation();
			FVector relativePlayerPos = _player->_chunk->GetActorTransform().InverseTransformPosition(playerPos);

			// Make sure left lane encompasses the area next to this player.
			if (relativePlayerPos.X >= (leftLane->GetUnscaledBoxExtent().X * 2.0f))
				return;



			_targetY = leftLane->GetRelativeLocation().Y;
			_moveToNewLane = true;

			// TIMELINE TEST
			//TArray<FRichCurveEditInfo> curves = _swapLaneCurve->GetCurves();
			//if (curves.Num() >= 3)
			//{
			//	FRichCurveEditInfo curve = curves[0];
			//	FRealCurve* realCurve = curve.CurveToEdit;
			//	realCurve->SetKeyValue(realCurve->GetFirstKeyHandle(), GetActorLocation().X);
			//	realCurve->SetKeyValue(realCurve->GetLastKeyHandle(), playerPos.X);

			//	curve = curves[1];
			//	realCurve = curve.CurveToEdit;
			//	realCurve->SetKeyValue(realCurve->GetFirstKeyHandle(), GetActorLocation().Y);
			//	realCurve->SetKeyValue(realCurve->GetLastKeyHandle(), playerPos.Y);


			//	_swapLaneTimeline->PlayFromStart();
			//}



			// Update current lane.
			UE_LOG(LogPlayerDefaultMoveState, Warning, TEXT("Lane changing from current: %s to target: %s"), *_currentLane->GetName(), *leftLane->GetName());
			_currentLane = leftLane;
			_laneSwitchDirection = ELaneSwitchDirection::LSD_LEFT;
		}
	}
}

void UPlayerDefaultMoveState::switchToRightLane()
{
	if (IsValid(_player->_chunk) && IsValid(_currentLane))
	{
		// Check if the current lane has a lane to the right of it.
		ULaneComponent* rightLane = _currentLane->getRightLane();
		if (IsValid(rightLane))
		{
			// Convert this player's world position to the local position of the current chunk underneath this player.
			FVector playerPos = _player->GetActorLocation();
			FVector relativePlayerPos = _player->_chunk->GetActorTransform().InverseTransformPosition(playerPos);

			// Make sure left lane encompasses the area next to this player.
			if (relativePlayerPos.X >= (rightLane->GetUnscaledBoxExtent().X * 2.0f))
				return;


			_targetY = rightLane->GetRelativeLocation().Y;
			_moveToNewLane = true;


			// Update current lane.
			UE_LOG(LogPlayerDefaultMoveState, Warning, TEXT("Lane changing from current: %s to target: %s"), *_currentLane->GetName(), *rightLane->GetName());
			_currentLane = rightLane;
			_laneSwitchDirection = ELaneSwitchDirection::LSD_RIGHT;
		}
	}
}

void UPlayerDefaultMoveState::turnLeft()
{
	AIntersectionChunk* chunk = Cast<AIntersectionChunk>(_player->_chunk);

	if (IsValid(chunk) && IsValid(_player->_levelGen))
	{
		ULaneComponent* topAltLane = chunk->getAltLanes().Top();
		float maxX = topAltLane->GetRelativeLocation().X + topAltLane->GetUnscaledBoxExtent().Y;
		FVector relativePos = chunk->GetActorTransform().InverseTransformPosition(_player->GetActorLocation());

		/*	Turn left if the following conditions are met.
		*	- The current chunk allows the player to turn left
		*	- The current lane is the far most left lane.
		*	- The player has not passed the top(right) most alt lane.
		*/
		if ((chunk->getChunckFeatures() & EChunkFeatures::CF_PLAYER_TURN_LEFT) && !IsValid(_currentLane->getLeftLane()) && (relativePos.X <= maxX))
		{
			UE_LOG(LogLaneCharacter, Error, TEXT("Trying to go left!"));

			_player->_levelGen->setMapOrientation(true);

			// Switch to the turning state.
			_transitionDirection = ETransitionDirection::TD_LEFT;
			_transitionState = ETransitionState::TS_TURNING;
		}
	}
}

void UPlayerDefaultMoveState::turnRight()
{
	AIntersectionChunk* chunk = Cast<AIntersectionChunk>(_player->_chunk);

	if (IsValid(chunk) && IsValid(_player->_levelGen))
	{
		ULaneComponent* topAltLane = chunk->getAltLanes().Top();
		float maxX = topAltLane->GetRelativeLocation().X + topAltLane->GetUnscaledBoxExtent().Y;
		FVector relativePos = chunk->GetActorTransform().InverseTransformPosition(_player->GetActorLocation());

		/*	Turn right if the following conditions are met.
		*	- The current chunk allows the player to turn right
		*	- The current lane is the far most right lane.
		*	- The player has not passed the top(right) most alt lane.
		*/
		if ((chunk->getChunckFeatures() & EChunkFeatures::CF_PLAYER_TURN_RIGHT) && !IsValid(_currentLane->getRightLane()) && (relativePos.X <= maxX))
		{
			UE_LOG(LogLaneCharacter, Error, TEXT("Trying to go right!"));

			_player->_levelGen->setMapOrientation(false);

			// Switch to the turning state.
			_transitionDirection = ETransitionDirection::TD_RIGHT;
			_transitionState = ETransitionState::TS_TURNING;
		}
	}
}
