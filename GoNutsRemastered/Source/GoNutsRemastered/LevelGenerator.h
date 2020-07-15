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
	uint8 _slots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Lane Settings")
	TSet<ESegmentTypes> _validSegmentTypes;
	//TSet<TSubclassOf<ALevelSegment>> _validSegments;

	UPROPERTY()
	uint8 _usedSlots;

	bool isFull() const 
	{
		return _usedSlots >= _slots;
	}

	bool getValidLaneSegment(TSet<FSegmentSpawnInfo>& validSegmentsSet, FSegmentSpawnInfo& spawnInfo)
	{
		TArray<FSegmentSpawnInfo> validSegmentsArr = validSegmentsSet.Array();

		// Extract all valid segment types from the level generator.
		TSet<ESegmentTypes> levelGenSegments;
		
		for (const FSegmentSpawnInfo& currSpawnInfo : validSegmentsArr)
		{
			levelGenSegments.Emplace(currSpawnInfo._segmentType);
		}
		

		// Try to randomly find a match with the valid segment types from the lane.
		TSet<ESegmentTypes> allValidSegmentTypes = _validSegmentTypes.Intersect(levelGenSegments);

		if (allValidSegmentTypes.Num() <= 0)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Could not find a valid segment type between the lane and the level generator!"));
			return false;
		}

		ESegmentTypes validSegmentType = allValidSegmentTypes.Array()[FMath::RandRange(0, allValidSegmentTypes.Num() - 1)];


		spawnInfo = FSegmentSpawnInfo(*validSegmentsArr.FindByPredicate([&validSegmentType](FSegmentSpawnInfo& spawnInfo) -> bool
			{
				return spawnInfo._segmentType == validSegmentType;
			}));

		return true;
	}
};


USTRUCT(Blueprintable)
struct FSegmentClassTypes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Segment Settings")
	TArray<TSubclassOf<ALevelSegment>> _segmentClassTypes;
};


USTRUCT(Blueprintable)
struct FSegmentTypeConnectInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Segment Settings")
	TSet<FSegmentSpawnInfo> _validRightSegments;			
															
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Segment Settings")
	TSet<FSegmentSpawnInfo> _validLeftSegments;				
															
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Segment Settings")
	TSet<FSegmentSpawnInfo> _validTopSegments;				
															
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Segment Settings")
	TSet<FSegmentSpawnInfo> _validBottomSegments;
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
	TMap<ESegmentTypes, FSegmentClassTypes>& getSegments() { return _segments; }

	const TSet<FSegmentSpawnInfo>& getValidRightSegments(const ESegmentTypes& segmentType) const;
	const TSet<FSegmentSpawnInfo>& getValidLeftSegments(const ESegmentTypes& segmentType) const;
	const TSet<FSegmentSpawnInfo>& getValidTopSegments(const ESegmentTypes& segmentType) const;
	const TSet<FSegmentSpawnInfo>& getValidBottomSegments(const ESegmentTypes& segmentType) const;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Segment Settings", meta = (AllowPrivateAccess = true))
	TMap<ESegmentTypes, FSegmentClassTypes> _segments;
	//TMultiMap<ESegmentTypes, TSubclassOf<ALevelSegment>> _segments;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Segment Settings", meta = (AllowPrivateAccess = true))
	TMap<ESegmentTypes, FSegmentTypeConnectInfo> _validSegmentsLookup;


	UFUNCTION()
	void updateLevelGen();
};
