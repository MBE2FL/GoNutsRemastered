// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMoveState.h"
#include "PlayerDefaultMoveState.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogPlayerDefaultMoveState, Log, All);

/**
 * 
 */
UCLASS()
class GONUTSREMASTERED_API UPlayerDefaultMoveState : public UPlayerMoveState
{
	GENERATED_BODY()
	
public:
	UPlayerDefaultMoveState();
	~UPlayerDefaultMoveState();

	virtual UPlayerMoveState* updateState() override;
	virtual void update(float deltaTime) override;
	virtual void switchToLeftLane() override;
	virtual void switchToRightLane() override;
	virtual void turnLeft() override;
	virtual void turnRight() override;
};
