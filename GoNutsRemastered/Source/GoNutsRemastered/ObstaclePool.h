// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ObstaclePool.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogObstaclePool, Log, All);

class AObstacle;
class UObstacleSpawner;

USTRUCT(NotBlueprintable)
struct FObstacleTypePool
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Object Memory Pool")
	TArray<AObstacle*> _objectPool;

	UPROPERTY(VisibleAnywhere, Category = "Object Memory Pool")
	uint8 _poolSize;
};

/**
 * 
 */
UCLASS()
class GONUTSREMASTERED_API UObstaclePool : public UObject
{
	GENERATED_BODY()

public:

	static UObstaclePool* getInstance();
	static void deleteInstance();
	void Init(UObstacleSpawner *obstacleSpawner);
	void createObjectPools();
	AObstacle* getObstacle(const TSubclassOf<AObstacle>& ObstacleClassType);
	void recycleObstacle(AObstacle* chunk);


private:

	UPROPERTY(VisibleAnywhere, Category = "Chunk Object Pool")
		TMap<TSubclassOf<AObstacle>, FObstacleTypePool> _objectTypePools;

	UPROPERTY()
	UObstacleSpawner* _ObstacleSpawner = nullptr;

	bool _initialized = false;

	static UObstaclePool* _instance;
	static const uint8 BASE_OBJECT_POOL_SIZE;
	static const uint8 POOL_SIZE_INCREASE;


	UObstaclePool() {};
	void deactivateChunk(AObstacle* obstacle);
	void activateChunk(AObstacle* obstacle);
	void addObjectsToPool(const uint8 numObjectsToAdd, FObstacleTypePool* objectTypePool, const TSubclassOf<AObstacle>& obstacleClassType);
};
