// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMoveState.h"
#include "PlayerTurningMoveState.generated.h"

class USpringArmComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerTurningMoveState, Log, All);

/**
 * 
 */
UCLASS()
class GONUTSREMASTERED_API UPlayerTurningMoveState : public UPlayerMoveState
{
	GENERATED_BODY()
	
public:
	UPlayerTurningMoveState();
	~UPlayerTurningMoveState();

	virtual void init(ALaneCharacter* player) override;
	virtual UPlayerMoveState* updateState() override;
	virtual void transition() override;
	virtual void update(float deltaTime) override;

private:
	void transitionRotatePlayer(float deltaTime);
	void transitionMovePlayer(float deltaTime);


	FQuat _startRot;
	FQuat _targetRot;
	USpringArmComponent* _springArm;

	FVector2D _a;
	FVector2D _b;
	FVector2D _c;
	FVector2D _d;
};
