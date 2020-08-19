// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTurningMoveState.h"
#include "PlayerAltLaneMoveState.h"
#include "LevelChunk.h"
#include "LaneCharacter.h"
#include "LaneComponent.h"
#include "LevelGenerator.h"
#include "IntersectionChunk.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY(LogPlayerTurningMoveState);

UPlayerTurningMoveState::UPlayerTurningMoveState()
{
}

UPlayerTurningMoveState::~UPlayerTurningMoveState()
{
}

void UPlayerTurningMoveState::init(ALaneCharacter* player)
{
	if (!_initialized && IsValid(player))
	{
		_player = player;

		TInlineComponentArray<USpringArmComponent*> springArmComponents;
		_player->GetComponents<USpringArmComponent>(springArmComponents);

		if (springArmComponents.IsValidIndex(0))
		{
			_springArm = springArmComponents[0];
		}
		else
		{
			UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Failed to initialize player turning state!: Could not find player's spring arm component!"));
			return;
		}

		_initialized = true;
	}
}

UPlayerMoveState* UPlayerTurningMoveState::updateState()
{
	switch (_transitionState)
	{
	case ETransitionState::TS_ALT_LANE:
	{
		_transitionState = ETransitionState::TS_NONE;
		UPlayerAltLaneMoveState* moveState = ALaneCharacter::getPlayerAltLaneMoveState();
		moveState->stateTransition(this);
		UE_LOG(LogPlayerTurningMoveState, Log, TEXT("STATE changed from turning to alt lane!"));
		return moveState;
	}
	default:
		return nullptr;
	}
}

void UPlayerTurningMoveState::transition()
{
	AIntersectionChunk* chunk = Cast<AIntersectionChunk>(_player->_chunk);

	if (IsValid(chunk))
	{
		// Get the middle lane in the intersection's alternative lanes.
		const TArray<ULaneComponent*> altLanes = chunk->getAltLanes();
		ULaneComponent* midLane = nullptr;

		// Use middle lane for chunks with an odd number of lanes.
		//if (FMath::Fmod(altLanes.Num(), 2) > 0.0f)
		if ((altLanes.Num() & 1) == 1)
		{
			midLane = altLanes[(altLanes.Num() * 0.5f)];
		}
		// Use the left middle lane for chunks with an even number of lanes.
		else
		{
			midLane = altLanes[(altLanes.Num() * 0.5f) - 1];
		}

		if (!IsValid(midLane))
		{
			UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Could not find middle alt lane to transition to!"));
			return;
		}



		FVector targetPos = midLane->GetRelativeLocation();
		FRotator startRotation = _player->GetActorRotation();
		FRotator targetRotation = startRotation;

		switch (_transitionDirection)
		{
		case ETransitionDirection::TD_LEFT:
			targetPos.Y = _currentLane->GetRelativeLocation().Y - _currentLane->GetUnscaledBoxExtent().Y - 300.0f;
			targetRotation.Yaw -= 90.0f;
			break;
		case ETransitionDirection::TD_RIGHT:
			targetPos.Y = _currentLane->GetRelativeLocation().Y + _currentLane->GetUnscaledBoxExtent().Y + 300.0f;
			targetRotation.Yaw += 90.0f;
			break;
		default:
			UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Invalid transition direction!"));
			break;
		}


		// Calculate rotaion variables.
		//SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
		//AddControllerYawInput(-90.0f);
		_startRot = startRotation.Quaternion();
		_targetRot = targetRotation.Quaternion();


		// Calculate position variables.
		FVector playerPos = _player->GetActorLocation();
		FVector relativePlayerPos = chunk->GetActorTransform().InverseTransformPosition(playerPos);
		relativePlayerPos.X = targetPos.X;
		relativePlayerPos.Y = targetPos.Y;
		playerPos = chunk->GetActorTransform().TransformPosition(relativePlayerPos);

		//_player->SetActorLocation(playerPos);
		_transitionStartPos = _player->GetActorLocation();
		_transitionTargetPos = playerPos;


		// Calculate Catmull-Rom coeffiecients.
		FVector2D P0 = FVector2D(_transitionStartPos);
		FVector2D P1 = FVector2D(_transitionStartPos);
		FVector2D P2 = FVector2D(_transitionTargetPos);
		FVector2D P3 = FVector2D(_transitionTargetPos);

		switch (_transitionDirection)
		{
		case ETransitionDirection::TD_LEFT:
			P0.Y += 500.0f;

			P3.Y -= 500.0f;
			break;
		case ETransitionDirection::TD_RIGHT:
			P0.Y -= 500.0f;

			P3.Y += 500.0f;
			break;
		default:
			UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Invalid transition direction!"));
			break;
		}

		if (P0.X <= _transitionTargetPos.X)
		{
			P0.X -= 500.0f;
		}
		else
		{
			P0.X += 500.0f;
		}

		float alpha = 0.5f;
		float tension = 0.0f;

		float t0 = 0.0f;
		float t1 = t0 + FMath::Pow(FVector2D::Distance(P0, P1), alpha);
		float t2 = t1 + FMath::Pow(FVector2D::Distance(P1, P2), alpha);
		float t3 = t2 + FMath::Pow(FVector2D::Distance(P2, P3), alpha);

		FVector2D m1 = (1.0f - tension) * (t2 - t1) *
			((P1 - P0) / (t1 - t0) - (P2 - P0) / (t2 - t0) + (P2 - P1) / (t2 - t1));
		FVector2D m2 = (1.0f - tension) * (t2 - t1) *
			((P2 - P1) / (t2 - t1) - (P3 - P1) / (t3 - t1) + (P3 - P2) / (t3 - t2));

		_a = 2.0f * (P1 - P2) + m1 + m2;
		_b = -3.0f * (P1 - P2) - m1 - m1 - m2;
		_c = m1;
		_d = P1;

		UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline P0: %s"), *P0.ToString());
		UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline START: %s"), *P1.ToString());
		UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline TARGET: %s"), *P2.ToString());
		UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline P3: %s"), *P3.ToString());


		// Set the current lane to the middle alt lane.
		_currentLane = midLane;

		_player->_rotatePlayer = true;

		UE_LOG(LogPlayerTurningMoveState, Log, TEXT("Current Lane switched to: %s"), *_currentLane->GetName());
	}
	else
	{
		UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Player could not transition to a new road on the intersection!"));
	}
}

void UPlayerTurningMoveState::update(float deltaTime)
{
	if (!_initialized)
	{
		return;
	}


	// Smoothly rotate this player to line up with the middle alt lane.
	// Move the character to the middle alt lane of the intersection.
	_transitionTValue += 1.0f * deltaTime;

	transitionRotatePlayer(deltaTime);
	transitionMovePlayer(deltaTime);


	// Move this character forward.
	_player->AddMovementInput(_player->GetActorForwardVector(), 0.5f);
}

void UPlayerTurningMoveState::transitionRotatePlayer(float deltaTime)
{
	// Tell the player and the player's spring arm component to stop using the movement component's control rotation, while transitioning.
	_player->bUseControllerRotationYaw = false;
	_springArm->bUsePawnControlRotation = false;

	FRotator currentRotation = _player->GetActorRotation();

	// Finish the rotation transition.
	if (_transitionTValue >= 1.0f)
	{
		_player->SetActorRotation(_targetRot.Rotator());

		// Start using the control rotation again.
		// Note: Also sets the control rotation's yaw to the target rotation's yaw.
		//_player->bUseControllerRotationYaw = true;
		_player->AddControllerYawInput(_targetRot.Rotator().Yaw);
		//_springArm->bUsePawnControlRotation = true;
	}
	// Transition to target rotation.
	else
	{
		currentRotation.Yaw = FQuat::Slerp(_startRot, _targetRot, _transitionTValue).Rotator().Yaw;

		_player->SetActorRotation(currentRotation);
	}

	//UE_LOG(LogPlayerDefaultMoveState, Warning, TEXT("Rotating: %s"), *_player->GetActorRotation().ToString());
}

void UPlayerTurningMoveState::transitionMovePlayer(float deltaTime)
{
	// Finish the position transition.
	if (_transitionTValue >= 1.0f)
	{
		// Reset transition interpolation value.
		_transitionTValue = 0.0f;

		_player->SetActorLocation(_transitionTargetPos);
		_player->_rotatePlayer = false;

		//_transitionDirection = ETransitionDirection::TD_NONE;
		_transitionState = ETransitionState::TS_ALT_LANE;
	}
	// Transition to target position.
	else
	{
		//_transitionCurrentPos = FMath::Lerp(_transitionStartPos, _transitionTargetPos, _transitionTValue);




#pragma region UE4_CATMULL-ROM
		//FVector P0 = _transitionStartPos;
		//P0.X -= 500.0f;
		////P0.X = FMath::Lerp(P0.X, _transitionTargetPos.X, 0.7);
		////P0.Y = FMath::Lerp(P0.Y, _transitionTargetPos.Y, 0.4);

		////FVector P3 = _transitionStartPos;
		//FVector P3 = _transitionTargetPos;
		//P3.Y += 500.0f;
		//
		////P3.X = FMath::Lerp(P3.X, _transitionTargetPos.X, 0.9);
		////P3.Y = FMath::Lerp(P3.Y, _transitionTargetPos.Y, 0.8);

		//float t0 = 0.0f;
		//float t1 = FMath::Pow(FVector::Distance(P0, _transitionStartPos), 0.5f);
		//float t2 = t1 + FMath::Pow(FVector::Distance(_transitionStartPos, _transitionTargetPos), 0.5f);
		//float t3 = t2 + FMath::Pow(FVector::Distance(_transitionTargetPos, P3), 0.5f);

		////t1 = 1;
		////t2 = 2;
		////t3 = 3;

		//_transitionCurrentPos = FMath::CubicCRSplineInterp(P0, _transitionStartPos, _transitionTargetPos, P3, t0, t1, t2, t3, _transitionTValue);

		//UE_LOG(LogPlayerDefaultMoveState, Error, TEXT(" "));
		//UE_LOG(LogPlayerDefaultMoveState, Error, TEXT("Spline: %s"), *_transitionCurrentPos.ToString());
		//UE_LOG(LogPlayerDefaultMoveState, Error, TEXT("Spline P0: %s"), *P0.ToString());
		//UE_LOG(LogPlayerDefaultMoveState, Error, TEXT("Spline START: %s"), *_transitionStartPos.ToString());
		//UE_LOG(LogPlayerDefaultMoveState, Error, TEXT("Spline TARGET: %s"), *_transitionTargetPos.ToString());
		//UE_LOG(LogPlayerDefaultMoveState, Error, TEXT("Spline P3: %s"), *P3.ToString());
		//UE_LOG(LogPlayerDefaultMoveState, Error, TEXT("Spline T0: %f"), t0);
		//UE_LOG(LogPlayerDefaultMoveState, Error, TEXT("Spline T1: %f"), t1);
		//UE_LOG(LogPlayerDefaultMoveState, Error, TEXT("Spline T2: %f"), t2);
		//UE_LOG(LogPlayerDefaultMoveState, Error, TEXT("Spline T3: %f"), t3);
#pragma endregion UE4_CATMULL_ROM



		//FVector2D P0 = FVector2D(_transitionStartPos);
		//P0.X -= 500.0f;
		//P0.Y -= 500.0f;

		//FVector2D P1 = FVector2D(_transitionStartPos);
		//FVector2D P2 = FVector2D(_transitionTargetPos);

		////FVector P3 = _transitionStartPos;
		//FVector2D P3 = FVector2D(_transitionTargetPos);
		//P3.Y += 500.0f;

		//float alpha = 0.5f;
		//float tension = 0.0f;

		//float t0 = 0.0f;
		//float t1 = t0 + FMath::Pow(FVector2D::Distance(P0, P1), alpha);
		//float t2 = t1 + FMath::Pow(FVector2D::Distance(P1, P2), alpha);
		//float t3 = t2 + FMath::Pow(FVector2D::Distance(P2, P3), alpha);

		//FVector2D m1 = (1.0f - tension) * (t2 - t1) *
		//	((P1 - P0) / (t1 - t0) - (P2 - P0) / (t2 - t0) + (P2 - P1) / (t2 - t1));
		//FVector2D m2 = (1.0f - tension) * (t2 - t1) *
		//	((P2 - P1) / (t2 - t1) - (P3 - P1) / (t3 - t1) + (P3 - P2) / (t3 - t2));

		//FVector2D a = 2.0f * (P1 - P2) + m1 + m2;
		//FVector2D b = -3.0f * (P1 - P2) - m1 - m1 - m2;
		//FVector2D c = m1;
		//FVector2D d = P1;

		FVector2D _currentPos = (_a * FMath::Pow(_transitionTValue, 3.0f)) + (_b * FMath::Pow(_transitionTValue, 2.0f)) + (_c * _transitionTValue) + _d;
		_transitionCurrentPos = FVector(_currentPos, _transitionTargetPos.Z);

		//UE_LOG(LogPlayerTurningMoveState, Error, TEXT(" "));
		UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline: %s"), *_currentPos.ToString());
		//UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline P0: %s"), *P0.ToString());
		//UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline START: %s"), *P1.ToString());
		//UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline TARGET: %s"), *P2.ToString());
		//UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline P3: %s"), *P3.ToString());
		//UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline T0: %f"), t0);
		//UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline T1: %f"), t1);
		//UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline T2: %f"), t2);
		//UE_LOG(LogPlayerTurningMoveState, Error, TEXT("Spline T3: %f"), t3);


		//_transitionCurrentPos = FMath::InterpEaseInOut(_transitionStartPos, _transitionTargetPos, _transitionTValue, 1.0f);

		_player->SetActorLocation(_transitionCurrentPos);
	}
}
