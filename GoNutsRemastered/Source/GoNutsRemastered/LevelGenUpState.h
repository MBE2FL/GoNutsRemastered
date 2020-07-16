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
	virtual void update() override;
	virtual ULevelGenState* updateState() override;

private:
	virtual ALevelChunk* getValidChunk() override;

};
