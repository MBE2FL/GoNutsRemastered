// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelGenState.h"
#include "LevelGenUpState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLevelGenUpState, Log, All);

/**
 * 
 */
UCLASS()
class GONUTSREMASTERED_API ULevelGenUpState : public ULevelGenState
{
	GENERATED_BODY()

public:
	virtual ULevelGenState* updateState() override;
	virtual void transition(const ALevelChunk* intersection, const EMapOrientations& prevMapOrientation) override;
	virtual void update() override;
};
