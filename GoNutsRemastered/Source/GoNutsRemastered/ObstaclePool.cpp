// Fill out your copyright notice in the Description page of Project Settings.

#include "ObstaclePool.h"
#include "Obstacle.h"
#include "ObstacleSpawner.h"

DEFINE_LOG_CATEGORY(LogObstaclePool);

UObstaclePool* UObstaclePool::_instance = nullptr;
const uint8 UObstaclePool::BASE_OBJECT_POOL_SIZE = 20;
const uint8 UObstaclePool::POOL_SIZE_INCREASE = 10;

UObstaclePool* UObstaclePool::getInstance()
{
	if (!IsValid(_instance))
	{
		_instance = NewObject<UObstaclePool>();
		_instance->AddToRoot();
	}

	return _instance;
}

void UObstaclePool::deleteInstance()
{
	if (IsValid(_instance))
	{
		_instance->ConditionalBeginDestroy();
		_instance = nullptr;
	}
}

void UObstaclePool::Init(UObstacleSpawner* obstacleSpawner)
{
	if (IsValid(obstacleSpawner))
	{
		_ObstacleSpawner = obstacleSpawner;

		createObjectPools();

		_initialized = true;
	}
	else
	{
		UE_LOG(LogObstaclePool, Error, TEXT("Obstacle object pool failed to initialize!"));
	}
}

void UObstaclePool::createObjectPools()
{
	const TMap<EObstacleType, FObstacleClassTypes> ObstacleClassTypes = _ObstacleSpawner->getObstacleClassType();
	TArray<TSubclassOf<AObstacle>> ObstacleClassTypesArr;
	//const TArray<TSubclassOf<ALevelChunk>> chunkClassTypes = _levelGen->getChunkClassTypes();

	for (const TPair<EObstacleType, FObstacleClassTypes>& ObstacleClassTypesInfo : ObstacleClassTypes)
	{
		ObstacleClassTypesArr = ObstacleClassTypesInfo.Value._obstacleClassTypes;

		// Create an object pool for each level chunk class type.
		for (const TSubclassOf<AObstacle>& ObstacleClassType : ObstacleClassTypesArr)
		{
			// Create objects for each chunk class type.
			FObstacleTypePool objectTypePool{};
			addObjectsToPool(BASE_OBJECT_POOL_SIZE, &objectTypePool, ObstacleClassType);

			// Add object pool to the memory pool.
			_objectTypePools.Emplace(ObstacleClassType, objectTypePool);

			UE_LOG(LogObstaclePool, Warning, TEXT("Added object type pool."));
		}
	}
}

AObstacle* UObstaclePool::getObstacle(const TSubclassOf<AObstacle>& ObstacleClassType)
{
	if (_initialized)
	{
		FObstacleTypePool* objectTypePool = _objectTypePools.Find(ObstacleClassType);

		if (objectTypePool)
		{
			TArray<AObstacle*>& obstacles = objectTypePool->_objectPool;

			// Dynamically increase the size of the object type pool, and return an element from it.
			if (obstacles.Num() <= 0)
			{
				addObjectsToPool(POOL_SIZE_INCREASE, objectTypePool, ObstacleClassType);

				AObstacle* obstacle = obstacles.Pop(false);
				activateChunk(obstacle);

				UE_LOG(LogObstaclePool, Warning, TEXT("Pool resized!: %s"), *ObstacleClassType.Get()->GetPathName());

				return obstacle;
			}
			// Return a chunk from the relevant object type pool.
			else
			{
				AObstacle* obstacle = obstacles.Pop(false);
				activateChunk(obstacle);
				return obstacle;
			}
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		UE_LOG(LogObstaclePool, Error, TEXT("Obstacle object pool not initialized, but trying to get an Obstacle!"));
		return nullptr;
	}
}

void UObstaclePool::recycleObstacle(AObstacle* obstacle)
{
	if (IsValid(obstacle))
	{
		TSubclassOf<AObstacle> obstacleClassType = obstacle->GetClass();

		FObstacleTypePool* objectPool = _objectTypePools.Find(obstacleClassType);

		// Level chunk type has a pool.
		if (objectPool)
		{
			// Pool is already full. Destroy chunk.
			if (objectPool->_objectPool.Num() >= objectPool->_poolSize)
			{
				UE_LOG(LogObstaclePool, Error, TEXT("Obstacle, %s's, object pool is already full, and cannot be recycled! Will Destroy Obstacle instead!"), *obstacle->GetName());
				_ObstacleSpawner->GetWorld()->DestroyActor(obstacle);
				return;
			}
			// Make sure chunk isn't already in it's pool.
			else if (objectPool->_objectPool.Contains(obstacle))
			{
				UE_LOG(LogObstaclePool, Warning, TEXT("Obstacle, %s of class type %s, is already in it's pool!"), *obstacle->GetName(), *obstacle->GetClass()->GetName());
				return;
			}
			// Recycle chunk.
			else
			{
				deactivateChunk(obstacle);
				objectPool->_objectPool.Emplace(obstacle);
				return;
			}
		}
		else
		{
			UE_LOG(LogObstaclePool, Error, TEXT("Obstacle, %s of class type %s, does not belong to any object pools, and cannot be recycled!"), *obstacle->GetName(), *obstacle->GetClass()->GetName());
		}
	}
	else
	{
		UE_LOG(LogObstaclePool, Error, TEXT("Invalid obstacle trying to be recycled!"));
	}
}

void UObstaclePool::deactivateChunk(AObstacle* obstacle)
{
	obstacle->SetActorHiddenInGame(true);
	obstacle->SetActorEnableCollision(false);
	obstacle->SetActorTickEnabled(false);
}

void UObstaclePool::activateChunk(AObstacle* obstacle)
{
	obstacle->SetActorHiddenInGame(false);
	obstacle->SetActorEnableCollision(true);
	obstacle->SetActorTickEnabled(true);
}

void UObstaclePool::addObjectsToPool(const uint8 numObjectsToAdd, FObstacleTypePool* objectTypePool, const TSubclassOf<AObstacle>& obstacleClassType)
{
	// Reserve space ahead of time.
	//objectPool->_objectPool.Reserve(objectPool->_objectPool.Max() + BASE_OBJECT_POOL_SIZE);
	objectTypePool->_objectPool.Reserve(objectTypePool->_poolSize + numObjectsToAdd);

	// Create the specified number of objects.
	AObstacle* obstacle = nullptr;
	for (uint8 count = 0; count < numObjectsToAdd; ++count)
	{
		// Spawn a chunk and add it to the appropriate object pool.
		obstacle = Cast<AObstacle>(_ObstacleSpawner->GetWorld()->SpawnActor(obstacleClassType));
		deactivateChunk(obstacle);

		objectTypePool->_objectPool.Emplace(obstacle);

		UE_LOG(LogObstaclePool, Warning, TEXT("Created obstacle: %s"), *obstacle->GetName());
	}

	objectTypePool->_poolSize += numObjectsToAdd;
}
