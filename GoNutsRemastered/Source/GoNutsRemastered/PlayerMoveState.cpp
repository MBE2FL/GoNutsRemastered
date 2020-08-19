// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveState.h"
#include "LevelChunk.h"
#include "LaneCharacter.h"
#include "LaneComponent.h"

DEFINE_LOG_CATEGORY(LogPlayerMoveState);

UPlayerMoveState::UPlayerMoveState()
{
	_laneSwitchDirection = ELaneSwitchDirection::LSD_NONE;
	_transitionDirection = ETransitionDirection::TD_NONE;
}

UPlayerMoveState::~UPlayerMoveState()
{
}

void UPlayerMoveState::init(ALaneCharacter* player)
{
	if (!_initialized && IsValid(player))
	{
		_player = player;
		_initialized = true;
	}
}

void UPlayerMoveState::stateTransition(UPlayerMoveState* moveState)
{
	_chunkChanged = false;
	_currentLane = moveState->_currentLane;
	_lastOverlappedLane = moveState->_lastOverlappedLane;


	_laneSwitchDirection = moveState->_laneSwitchDirection;
	_targetY = moveState->_targetY;
	_moveToNewLane = moveState->_moveToNewLane;
	_transitionDirection = moveState->_transitionDirection;


	transition();
}

void UPlayerMoveState::onBeginOverlap(AActor* otherActor, UPrimitiveComponent* otherComp)
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
			_player->_chunk = Cast<ALevelChunk>(otherActor);
			_chunkChanged = true;
			UE_LOG(LogPlayerMoveState, Warning, TEXT("Chunk underneath changed to %s."), *_player->_chunk->GetName());
		}
		break;
	default:
		break;
	}
}
