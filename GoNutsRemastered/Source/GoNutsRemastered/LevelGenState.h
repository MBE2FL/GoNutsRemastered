// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelGenState.generated.h"


class ALevelGenerator;
enum class EMapOrientations : uint8;
class ALevelChunk;
class ACustomCharacter;


DECLARE_LOG_CATEGORY_EXTERN(LogLevelGenState, Log, All);


/**
 * 
 */
UCLASS(Abstract)
class GONUTSREMASTERED_API ULevelGenState : public UObject
{
	GENERATED_BODY()

public:
	void init(ALevelGenerator* levelGen);
	virtual void cleanupState();

	virtual ULevelGenState* updateState() { return nullptr; };
	virtual void transition(const ALevelChunk* intersection, const EMapOrientations& prevMapOrientation) {};
	virtual void update() {};

protected:
	UPROPERTY()
	bool _initialized = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level Gen State", meta = (AllowPrivateAccess = true))
	ALevelGenerator* _levelGen = nullptr;
	UPROPERTY()
	int32 _currColumn;
	UPROPERTY()
	ALevelChunk* _prevChunk = nullptr;
	UPROPERTY()
	ACustomCharacter* _player = nullptr;

	ULevelGenState() {};
	virtual ALevelChunk* getValidChunk();
};
