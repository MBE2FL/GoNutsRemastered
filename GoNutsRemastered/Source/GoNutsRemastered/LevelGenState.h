// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelGenerator.h"
#include "LevelGenState.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class GONUTSREMASTERED_API ULevelGenState : public UObject
{
	GENERATED_BODY()

public:
	void init(ALevelGenerator* levelGen);

	virtual void update() {};
	virtual ULevelGenState* updateState() { return nullptr; };

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level Gen State", meta = (AllowPrivateAccess = true))
	ALevelGenerator* _levelGen;
	UPROPERTY()
	int32 _width = 6;
	UPROPERTY()
	int32 _currColumn;
	UPROPERTY()
	ALevelSegment* _prevRowStart;
	UPROPERTY()
	ALevelSegment* _currSegment = nullptr;
	UPROPERTY()
	ALevelSegment* prevHSegment = nullptr;


	ULevelGenState() {};
	virtual ALevelSegment* getValidSegment(ALevelSegment* leftSegment, FLane* lane) { return nullptr; };
	virtual void setValidOrientation(ALevelSegment* segment, uint8 validOrientations) {};
};
