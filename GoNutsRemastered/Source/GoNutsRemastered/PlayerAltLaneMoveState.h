// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMoveState.h"
#include "PlayerAltLaneMoveState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerAltLaneMoveState, Log, All);

/**
 * 
 */
UCLASS()
class GONUTSREMASTERED_API UPlayerAltLaneMoveState : public UPlayerMoveState
{
	GENERATED_BODY()
	
public:
	UPlayerAltLaneMoveState();
	~UPlayerAltLaneMoveState();

	virtual UPlayerMoveState* updateState() override;
	virtual void transition() override;
	virtual void update(float deltaTime) override;
	virtual void switchToLeftLane() override;
	virtual void switchToRightLane() override;
	virtual void onBeginOverlap(AActor* otherActor, UPrimitiveComponent* otherComp) override;
};
