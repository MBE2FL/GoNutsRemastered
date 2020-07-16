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
	virtual ALevelChunk* getValidChunk() { return nullptr; };
};
