// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChunkObjectPool.generated.h"


class ALevelGenerator;
class ALevelChunk;
//class ACharacter;
class AFreeRoamCharacter;


DECLARE_LOG_CATEGORY_EXTERN(LogChunkObjectPool, Log, All);


USTRUCT(NotBlueprintable)
struct FChunkObjectTypePool
{
	GENERATED_BODY()

	//UPROPERTY(VisibleAnywhere, Category = "Chunk Memory Pool")
	//TMap<TSubclassOf<ALevelChunk>, ALevelChunk*> _objectPool;

	UPROPERTY(VisibleAnywhere, Category = "Chunk Memory Pool")
	TArray<ALevelChunk*> _objectPool;

	UPROPERTY(VisibleAnywhere, Category = "Chunk Memory Pool")
	uint8 _poolSize;
};


/**
 * 
 */
UCLASS()
class GONUTSREMASTERED_API UChunkObjectPool : public UObject
{
	GENERATED_BODY()
	
public:
	static UChunkObjectPool* getInstance();
	static void destroyInstance();
	AFreeRoamCharacter* getPlayer() const;
	void init(ALevelGenerator* levelGen);
	void createObjectPools();
	ALevelChunk* getLevelChunk(const TSubclassOf<ALevelChunk>& chunkClassType);
	void recycleLevelChunk(ALevelChunk* chunk);

private:

	UPROPERTY(VisibleAnywhere, Category = "Chunk Object Pool")
	TMap<TSubclassOf<ALevelChunk>, FChunkObjectTypePool> _objectTypePools;

	UPROPERTY()
	ALevelGenerator* _levelGen = nullptr;

	bool _initialized = false;

	static UChunkObjectPool* _instance;
	static const uint8 BASE_OBJECT_POOL_SIZE;
	static const uint8 POOL_SIZE_INCREASE;


	UChunkObjectPool() {};
	void deactivateChunk(ALevelChunk* chunk);
	void activateChunk(ALevelChunk* chunk);
	void addObjectsToPool(const uint8 numObjectsToAdd, FChunkObjectTypePool* objectTypePool, const TSubclassOf<ALevelChunk>& chunkClassType);
};
