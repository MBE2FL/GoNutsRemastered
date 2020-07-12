// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSegment.h"
#include "LevelGenerator.generated.h"


class ULevelGenState;
class ULevelGenUpState;
class ULevelGenLeftState;


#define ECC_LevelSegmentChannel ECollisionChannel::ECC_GameTraceChannel1


DECLARE_LOG_CATEGORY_EXTERN(LogLevelGen, Log, All);


UENUM(BlueprintType)
enum class EMapOrientations : uint8
{
	MO_Right,
	MO_Left,
	MO_Up,
	MO_Down
};

USTRUCT(Blueprintable)
struct FLane
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Lane Settings")
	float _width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Lane Settings")
	TSet<TSubclassOf<ALevelSegment>> _validSegments;
	UPROPERTY()
	float _usedWidth;

	bool isFull() const 
	{
		return _usedWidth >= (_width - 5.0f);
	}

	bool getValidLaneSegment(TSet<FSegmentSpawnInfo>& validSegmentsSet, FSegmentSpawnInfo& spawnInfo)
	{
		TArray<FSegmentSpawnInfo> validSegmentsArr = validSegmentsSet.Array();

		// Extract all valid segments from the level generator.
		TSet<TSubclassOf<ALevelSegment>> levelGenSegments;
		
		for (const FSegmentSpawnInfo& currSpawnInfo : validSegmentsArr)
		{
			levelGenSegments.Emplace(currSpawnInfo._segment);
		}
		

		// Try to randomly find a match with the valid segments from the lane.
		TSet<TSubclassOf<ALevelSegment>> allValidSegments = _validSegments.Intersect(levelGenSegments);

		if (allValidSegments.Num() <= 0)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Could not find a valid segment between the lane and the level generator!"));
			return false;
		}

		TSubclassOf<ALevelSegment> validSegment = allValidSegments.Array()[FMath::RandRange(0, allValidSegments.Num() - 1)];


		spawnInfo = FSegmentSpawnInfo(*validSegmentsArr.FindByPredicate([&validSegment](FSegmentSpawnInfo& spawnInfo) -> bool
			{
				return spawnInfo._segment == validSegment;
			}));

		return true;
	}
};


//DECLARE_EVENT_OneParam(ALevelGenerator, FOnCrosswalkSpawned, AActor*)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCrosswalkSpawned, ALevelSegment*, crosswalk, const TArray<USceneComponent*>&, pedestrianSpawnPoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRoadSpawned, ALevelSegment*, road, const TArray<USceneComponent*>&, obstacleSpawnPoints);

UCLASS(Blueprintable)
class GONUTSREMASTERED_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

	FOnCrosswalkSpawned& onCrosswalkSpawned() { return _onCrosswalkSpawnedEvent; }
	FOnRoadSpawned& onRoadSpawned() { return _onRoadSpawnedEvent; }

	EMapOrientations getMapOrientation() const { return _mapOrientation; };
	int32 getWidth() const { return _width; };
	static ULevelGenUpState* getLevelGenUpState() { return _levelGenUpState; };
	static ULevelGenLeftState* getLevelGenLeftState() { return _levelGenLeftState; };
	TArray<FLane>& getLanes() { return _lanes; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(BlueprintAssignable, Category = "Level Gen|Generation Events", meta = (AllowPrivateAccess = true))
	FOnCrosswalkSpawned _onCrosswalkSpawnedEvent;

	UPROPERTY(BlueprintAssignable, Category = "Level Gen|Generation Events", meta = (AllowPrivateAccess = true))
	FOnRoadSpawned _onRoadSpawnedEvent;



	UPROPERTY()
	ALevelSegment* _prevRowStart;
	UPROPERTY()
	EMapOrientations _mapOrientation = EMapOrientations::MO_Up;
	UPROPERTY()
	int32 _width = 6;
	UPROPERTY()
	ULevelGenState* _levelGenState;
	static ULevelGenUpState* _levelGenUpState;
	static ULevelGenLeftState* _levelGenLeftState;
	UPROPERTY()
	FTimerHandle _timerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Lane Settings", meta = (AllowPrivateAccess = true))
	TArray<FLane> _lanes;


	UFUNCTION()
	void updateLevelGen();
};
