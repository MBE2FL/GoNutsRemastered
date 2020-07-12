// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelGenState.h"
#include "LevelGenLeftState.generated.h"

/**
 * 
 */
UCLASS()
class GONUTSREMASTERED_API ULevelGenLeftState : public ULevelGenState
{
	GENERATED_BODY()
	
public:
	virtual void update() override;
	virtual ULevelGenState* updateState() override;

private:
	virtual ALevelSegment* getValidSegment(ALevelSegment* leftSegment, FLane* lane) override;
	virtual void setValidOrientation(ALevelSegment* segment, uint8 validOrientations) override;
};
