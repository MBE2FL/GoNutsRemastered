// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelGenState.h"
#include "LevelGenLeftState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLevelGenLeftState, Log, All);

/**
 * 
 */
UCLASS()
class GONUTSREMASTERED_API ULevelGenLeftState : public ULevelGenState
{
	GENERATED_BODY()
	
public:
	virtual ULevelGenState* updateState() override;
	virtual void update() override;
	virtual void transition(const ALevelChunk* intersection) override;

private:
	virtual ALevelChunk* getValidChunk() override;
};
