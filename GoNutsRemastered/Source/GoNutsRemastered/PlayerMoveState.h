// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerMoveState.generated.h"

class ALevelChunk;
class ULaneComponent;
enum class ELaneSwitchDirection : uint8;
enum class ETransitionDirection : uint8;
class ALaneCharacter;


DECLARE_LOG_CATEGORY_EXTERN(LogPlayerMoveState, Log, All);


UENUM(BlueprintType)
enum class ETransitionState : uint8
{
	TS_NONE,
	TS_DEFAULT,
	TS_TURNING,
	TS_ALT_LANE
};


/**
 * 
 */
UCLASS(Abstract)
class GONUTSREMASTERED_API UPlayerMoveState : public UObject
{
	GENERATED_BODY()

public:
	UPlayerMoveState();
	virtual ~UPlayerMoveState();

	virtual void init(ALaneCharacter* player);
	virtual UPlayerMoveState* updateState() { return nullptr; };
	virtual void stateTransition(UPlayerMoveState* moveState);
	virtual void transition() {};
	virtual void update(float deltaTime) {};
	virtual void switchToLeftLane() {};
	virtual void switchToRightLane() {};
	virtual void turnLeft() {};
	virtual void turnRight() {};
	virtual void onBeginOverlap(AActor* otherActor, UPrimitiveComponent* otherComp);


protected:
	bool _initialized = false;
	ETransitionState _transitionState = ETransitionState::TS_NONE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	ALaneCharacter* _player = nullptr;
	bool _chunkChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	ULaneComponent* _currentLane = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	ULaneComponent* _lastOverlappedLane;


	ELaneSwitchDirection _laneSwitchDirection;
	float _targetY;
	bool _moveToNewLane = false;
	float _laneSwitchSpeed = 18.0f;
	ETransitionDirection _transitionDirection;


	bool _transition = false;
	FVector _transitionStartPos;
	FVector _transitionCurrentPos;
	FVector _transitionTargetPos;
	float _transitionTValue = 0.0f;
};
