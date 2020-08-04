// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChunk.h"
#include "ChunkObjectPool.h"
#include "FreeRoamCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "LaneComponent.h"


DEFINE_LOG_CATEGORY(LogLevelChunk);


// Sets default values
ALevelChunk::ALevelChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	_mesh->SetStaticMesh(meshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> matAsset(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	_mesh->SetMaterial(0, matAsset.Object);
	_mesh->SetCollisionProfileName(TEXT("Custom"));
	_mesh->SetCollisionObjectType(ECC_LevelChunkChannel);
	_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	RootComponent = _mesh;
}

// Called when the game starts or when spawned
void ALevelChunk::BeginPlay()
{
	Super::BeginPlay();

	
	// Store all lanes in a TArray for easy access.
	TInlineComponentArray<ULaneComponent*> laneComponents(this);
	GetComponents(laneComponents);

	_lanes.Reserve(laneComponents.Num());
	_lanes = laneComponents;
}

// Called every frame
void ALevelChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


#if WITH_EDITOR
	//if (!GWorld->HasBegunPlay())
	//UWorld* world = GEngine->GetWorldFromContextObject(this);
	UWorld* world = Cast<UObject>(this)->GetWorld();

	if (IsValid(world))
	{
		if (world->WorldType == EWorldType::Type::EditorPreview)
		{
			return;
		}
	}
	else
	{
		return;
	}
#endif


	UChunkObjectPool* chunkObjectPool = UChunkObjectPool::getInstance();
	AFreeRoamCharacter* player = chunkObjectPool->getPlayer();

	if (IsValid(player))
	{
		FVector nonNormDir = GetActorLocation() - player->GetActorLocation();

		// Check if this chunk is behind the player.
		if (FVector::DotProduct(player->GetActorForwardVector(), nonNormDir) < 0.0f)
		{
			// Check if this chunk is far enough away from the player to be recycled.
			// Distance is calculated from the player's position to this chunks's position, minus this chunks's static local vertical(X) size.
			float offset = FMath::Square(getMesh()->GetStaticMesh()->GetBounds().BoxExtent.X * 2.0f);
			if (FVector::DistSquaredXY(player->GetActorLocation(), GetActorLocation()) - offset >= FMath::Square(2000.0f))
			{
				chunkObjectPool->recycleLevelChunk(this);

				// Recycle all obstacles on this chunk.
				for (ULaneComponent* lane : _lanes)
				{
					lane->recycleObstacles();
				}
			}
		}

	}
	else
	{
		UE_LOG(LogLevelChunk, Error, TEXT("Chunk, %s, does not have a reference to the player!"), *GetName());
	}
}

EChunkDescriptors ALevelChunk::getChunkDescriptor() const
{
	return _chunkDescriptor;
}

EChunkFeatures::Type ALevelChunk::getChunckFeatures() const
{
	return static_cast<EChunkFeatures::Type>(_chunkFeatures);
}

UStaticMeshComponent* ALevelChunk::getMesh() const
{
	return _mesh;
}

const TArray<ULaneComponent*>& ALevelChunk::getLanes() const
{
	return _lanes;
}
