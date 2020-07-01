// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSegment.h"
#include "LevelGenerator.generated.h"

//class ALevelSegment;


//USTRUCT(BlueprintType)
//struct FSegmentGroup
//{
//	GENERATED_BODY()
//
//	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
//	//TArray<TSubclassOf<ALevelSegment>> _validSegments;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
//	TSet<TSubclassOf<ALevelSegment>> _validRightSegments;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
//	TSet<TSubclassOf<ALevelSegment>> _validLeftSegments;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
//	TSet<TSubclassOf<ALevelSegment>> _validUpSegments;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
//	TSet<TSubclassOf<ALevelSegment>> _validDownSegments;
//};


DECLARE_LOG_CATEGORY_EXTERN(LogLevelGen, Log, All);


//DECLARE_EVENT_OneParam(ALevelGenerator, FOnCrosswalkSpawned, AActor*)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCrosswalkSpawned, ALevelSegment*, crosswalk);

UCLASS(Blueprintable)
class GONUTSREMASTERED_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

	FOnCrosswalkSpawned& onCrosswalkSpawned() { return _onCrosswalkSpawnedEvent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	void randomSpawn();

	void streamLevelsTest();

	ALevelSegment* getValidSegment(ESegmentTypes hSegmentType, ESegmentTypes vSegmentType);
	ALevelSegment* getValidSegment(ALevelSegment* hSegment, ALevelSegment* vSegment);
	ALevelSegment* getValidSegment(ALevelSegment* hSegment, ALevelSegment* vSegment, int ver);
	void setValidOrientation(ALevelSegment* segment, ESegmentOrientations leftOrientation, ESegmentTypes leftType,
								ESegmentTypes bottomType, ESegmentOrientations bottomOrientation);
	void setValidOrientation(ALevelSegment* segment, uint8 validOrientations);


	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Level Gen|Generation Events", meta = (AllowPrivateAccess = true))
	FOnCrosswalkSpawned _onCrosswalkSpawnedEvent;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	//TArray<AActor*> _spawnableActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Grid Settings", meta = (AllowPrivateAccess = true))
	int32 _numRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Grid Settings", meta = (AllowPrivateAccess = true))
	int32 _numColumns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen", meta = (AllowPrivateAccess = true))
	TArray<FName> _levelStreamNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<ALevelSegment>> _spawnableActors;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	//TMap<ESegmentTypes, TArray<TSubclassOf<ALevelSegment>>> _segmentGroups;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	TMap<ESegmentTypes, FSegmentGroup> _segmentGroups;
};
