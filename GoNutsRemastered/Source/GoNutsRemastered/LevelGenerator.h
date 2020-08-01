// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "LevelSegment.h"
#include "LevelChunk.h"
#include "LevelGenerator.generated.h"


class ULevelGenState;
class ULevelGenUpState;
class ULevelGenLeftState;
class ULevelGenRightState;
class ULevelGenDownState;
class UChunkObjectPool;
//class ACharacter;
class AFreeRoamCharacter;


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
struct FChunkClassTypes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Chunk Settings")
	TArray<TSubclassOf<ALevelChunk>> _chunkClassTypes;
};


//USTRUCT(Blueprintable)
//struct FSegmentTypeConnectInfo
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Segment Settings")
//	TSet<FSegmentSpawnInfo> _validRightSegments;			
//};


//DECLARE_EVENT_OneParam(ALevelGenerator, FOnCrosswalkSpawned, AActor*)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCrosswalkSpawned, ALevelChunk*, crosswalk, const TArray<USceneComponent*>&, pedestrianSpawnPoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRoadSpawned, ALevelChunk*, road, const TArray<USceneComponent*>&, obstacleSpawnPoints);

UCLASS(Blueprintable)
class GONUTSREMASTERED_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	FOnCrosswalkSpawned& onCrosswalkSpawned() { return _onCrosswalkSpawnedEvent; }
	FOnRoadSpawned& onRoadSpawned() { return _onRoadSpawnedEvent; }

	EMapOrientations getMapOrientation() const { return _mapOrientation; };
	void setMapOrientation(const bool& turnLeft);
	static ULevelGenUpState* getLevelGenUpState() { return _levelGenUpState; };
	static ULevelGenLeftState* getLevelGenLeftState() { return _levelGenLeftState; };
	static ULevelGenRightState* getLevelGenRightState() { return _levelGenRightState; };
	static ULevelGenDownState* getLevelGenDownState() { return _levelGenDownState; };

	const TMap<int32, FChunkClassTypes>& getChunkClassTypes() const { return _chunks; }
	//const TArray<TSubclassOf<ALevelChunk>>& getChunkClassTypes() const { return _chunkClassTypes; }
	



	// Chunk memory pool funcitons
	ALevelChunk* spawnChunk(const TSubclassOf<ALevelChunk>& chunkClassType);
	void recycleChunk(ALevelChunk* chunk);


	AFreeRoamCharacter* getPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(BlueprintAssignable, Category = "Level Gen|Generation Events", meta = (AllowPrivateAccess = true))
	FOnCrosswalkSpawned _onCrosswalkSpawnedEvent;

	UPROPERTY(BlueprintAssignable, Category = "Level Gen|Generation Events", meta = (AllowPrivateAccess = true))
	FOnRoadSpawned _onRoadSpawnedEvent;



	UPROPERTY()
	EMapOrientations _mapOrientation = EMapOrientations::MO_Up;

	UPROPERTY()
	EMapOrientations _prevMapOrientation = EMapOrientations::MO_Up;

	UPROPERTY()
	ULevelGenState* _levelGenState;
	//TWeakObjectPtr<ULevelGenState> _levelGenState;

	static ULevelGenUpState* _levelGenUpState;
	static ULevelGenLeftState* _levelGenLeftState;
	static ULevelGenRightState* _levelGenRightState;
	static ULevelGenDownState* _levelGenDownState;

	//UPROPERTY()
	//FTimerHandle _timerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Chunk Settings", meta = (AllowPrivateAccess = true))
	TMap<int32, FChunkClassTypes> _chunks;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Chunk Settings", meta = (AllowPrivateAccess = true))
	//TArray<TSubclassOf<ALevelChunk>> _chunkClassTypes;

	UPROPERTY()
	UChunkObjectPool* _chunkObjectPool;

	UPROPERTY()
	AFreeRoamCharacter* _player;

	UFUNCTION()
	void updateLevelGen();

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Chunk Settings", meta = (AllowPrivateAccess = true))
	bool _refreshChunkClassTypes = false;
#endif

#if WITH_EDITOR
	UFUNCTION()
	void getAllChunkClassTypes();
#endif
};
