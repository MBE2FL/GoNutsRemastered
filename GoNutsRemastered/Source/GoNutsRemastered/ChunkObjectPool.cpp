// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkObjectPool.h"
#include "LevelGenerator.h"
#include "LevelChunk.h"
#include "CustomCharacter.h"

DEFINE_LOG_CATEGORY(LogChunkObjectPool);

UChunkObjectPool* UChunkObjectPool::_instance = nullptr;
const uint8 UChunkObjectPool::BASE_OBJECT_POOL_SIZE = 5;
const uint8 UChunkObjectPool::POOL_SIZE_INCREASE = 5;

#define DEFAULT_SPAWN_POS FVector(0.0f, 0.0f, 2000.0f)


UChunkObjectPool* UChunkObjectPool::getInstance()
{
	if (!IsValid(_instance))
	{
		_instance = NewObject<UChunkObjectPool>();	// TO-DO Add to the root set of objects.
	}

	return _instance;
}

void UChunkObjectPool::destroyInstance()
{
	if (IsValid(_instance))
	{
		_instance->ConditionalBeginDestroy();
		_instance = nullptr;
	}
}

ACustomCharacter* UChunkObjectPool::getPlayer() const
{
	if (IsValid(_levelGen))
	{
		return _levelGen->getPlayer();
	}
	else
	{
		return nullptr;
	}
}

void UChunkObjectPool::init(ALevelGenerator* levelGen)
{
	if (IsValid(levelGen))
	{
		_levelGen = levelGen;

		createObjectPools();

		_initialized = true;
	}
	else
	{
		UE_LOG(LogChunkObjectPool, Error, TEXT("Chunk object pool failed to initialize!"));
	}
}

void UChunkObjectPool::createObjectPools()
{
	const TMap<EChunkDescriptors, FChunkClassTypes> chunkClassTypes = _levelGen->getChunkClassTypes();
	TArray<TSubclassOf<ALevelChunk>> chunkClassTypesArr;
	//const TArray<TSubclassOf<ALevelChunk>> chunkClassTypes = _levelGen->getChunkClassTypes();

	for (const TPair<EChunkDescriptors, FChunkClassTypes>& chunkClassTypesInfo : chunkClassTypes)
	{
		chunkClassTypesArr = chunkClassTypesInfo.Value._chunkClassTypes;

		// Create an object pool for each level chunk class type.
		for (const TSubclassOf<ALevelChunk>& chunkClassType : chunkClassTypesArr)
		{
			// Create objects for each chunk class type.
			FChunkObjectTypePool objectTypePool{};
			addObjectsToPool(BASE_OBJECT_POOL_SIZE, &objectTypePool, chunkClassType);

			// Add object pool to the memory pool.
			_objectTypePools.Emplace(chunkClassType, objectTypePool);

			UE_LOG(LogChunkObjectPool, Warning, TEXT("Added object type pool."));
		}
	}
}

ALevelChunk* UChunkObjectPool::getLevelChunk(const TSubclassOf<ALevelChunk>& chunkClassType)
{
	if (_initialized)
	{
		FChunkObjectTypePool* objectTypePool = _objectTypePools.Find(chunkClassType);

		if (objectTypePool)
		{
			TArray<ALevelChunk*>& chunks = objectTypePool->_objectPool;

			// Dynamically increase the size of the object type pool, and return an element from it.
			if (chunks.Num() <= 0)
			{
				addObjectsToPool(POOL_SIZE_INCREASE, objectTypePool, chunkClassType);

				ALevelChunk* chunk = chunks.Pop(false);
				activateChunk(chunk);

				UE_LOG(LogChunkObjectPool, Warning, TEXT("Pool resized!: %s"), *chunkClassType.Get()->GetPathName());

				return chunk;
			}
			// Return a chunk from the relevant object type pool.
			else
			{
				ALevelChunk* chunk = chunks.Pop(false);
				activateChunk(chunk);
				return chunk;
			}
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		UE_LOG(LogChunkObjectPool, Error, TEXT("Chunk object pool not initialized, but trying to get a level chunk!"));
		return nullptr;
	}
}

void UChunkObjectPool::recycleLevelChunk(ALevelChunk* chunk)
{
	if (IsValid(chunk))
	{
		TSubclassOf<ALevelChunk> chunkClassType = chunk->GetClass();

		FChunkObjectTypePool* objectPool = _objectTypePools.Find(chunkClassType);

		// Level chunk type has a pool.
		if (objectPool)
		{
			// Pool is already full. Destroy chunk.
			if (objectPool->_objectPool.Num() >= objectPool->_poolSize)
			{
				UE_LOG(LogChunkObjectPool, Error, TEXT("Chunk, %s's, object pool is already full, and cannot be recycled! Will Destroy chunk instead!"), *chunk->GetName());
				_levelGen->GetWorld()->DestroyActor(chunk);
				return;
			}
			// Make sure chunk isn't already in it's pool.
			else if (objectPool->_objectPool.Contains(chunk))
			{
				UE_LOG(LogChunkObjectPool, Warning, TEXT("Chunk, %s of class type %s, is already in it's pool!"), *chunk->GetName(), *chunk->GetClass()->GetName());
				return;
			}
			// Recycle chunk.
			else
			{
				deactivateChunk(chunk);
				objectPool->_objectPool.Emplace(chunk);
				//UE_LOG(LogChunkObjectPool, Warning, TEXT("Chunk, %s of class type %s, recycled!"), *chunk->GetName(), *chunk->GetClass()->GetName());
				return;
			}
		}
		else
		{
			UE_LOG(LogChunkObjectPool, Error, TEXT("Chunk, %s of class type %s, does not belong to any object pools, and cannot be recycled!"), *chunk->GetName(), *chunk->GetClass()->GetName());
		}
	}
	else
	{
		UE_LOG(LogChunkObjectPool, Error, TEXT("Invalid chunk trying to be recycled!"));
	}
}

void UChunkObjectPool::deactivateChunk(ALevelChunk* chunk)
{
	chunk->SetActorHiddenInGame(true);
	chunk->SetActorEnableCollision(false);
	chunk->SetActorTickEnabled(false);
	chunk->SetActorLocation(DEFAULT_SPAWN_POS);
}

void UChunkObjectPool::activateChunk(ALevelChunk* chunk)
{
	chunk->SetActorHiddenInGame(false);
	chunk->SetActorEnableCollision(true);
	chunk->SetActorTickEnabled(true);
}

void UChunkObjectPool::addObjectsToPool(const uint8 numObjectsToAdd, FChunkObjectTypePool* objectTypePool, const TSubclassOf<ALevelChunk>& chunkClassType)
{
	// Reserve space ahead of time.
	//objectPool->_objectPool.Reserve(objectPool->_objectPool.Max() + BASE_OBJECT_POOL_SIZE);
	objectTypePool->_objectPool.Reserve(objectTypePool->_poolSize + numObjectsToAdd);

	// Create the specified number of objects.
	ALevelChunk* chunk = nullptr;
	for (uint8 count = 0; count < numObjectsToAdd; ++count)
	{
		// Spawn a chunk and add it to the appropriate object pool.
		chunk = Cast<ALevelChunk>(_levelGen->GetWorld()->SpawnActor(chunkClassType));
		deactivateChunk(chunk);

		objectTypePool->_objectPool.Emplace(chunk);

		UE_LOG(LogChunkObjectPool, Warning, TEXT("Created chunk: %s"), *chunk->GetName());
	}

	objectTypePool->_poolSize += numObjectsToAdd;
}
