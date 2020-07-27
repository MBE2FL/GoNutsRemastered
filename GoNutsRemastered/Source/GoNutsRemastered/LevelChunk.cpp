// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChunk.h"
#include "ChunkObjectPool.h"
//#include "GameFramework/Character.h"
#include "FreeRoamCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"

DEFINE_LOG_CATEGORY(LogLevelChunk);

const EChunkDescriptors::Type ALevelChunk::TOWN_THREE_LANES_ISLAND = static_cast<EChunkDescriptors::Type>(EChunkDescriptors::Type::CD_BIOME_TYPE_TOWN | 
																		EChunkDescriptors::Type::CD_THREE_LANES | EChunkDescriptors::Type::CD_LANE_CONTAINS_ISLAND);
const EChunkDescriptors::Type ALevelChunk::TOWN_THREE_LANES_INTERSECTION = static_cast<EChunkDescriptors::Type>(EChunkDescriptors::Type::CD_BIOME_TYPE_TOWN |
																		EChunkDescriptors::Type::CD_THREE_LANES | EChunkDescriptors::Type::CD_IS_INTERSECTION);
const EChunkDescriptors::Type ALevelChunk::TOWN_TWO_LANES = static_cast<EChunkDescriptors::Type>(EChunkDescriptors::Type::CD_BIOME_TYPE_TOWN |
																		EChunkDescriptors::Type::CD_TWO_LANES);
const EChunkDescriptors::Type ALevelChunk::TOWN_THREE_TO_TWO_LANES_MERGER = static_cast<EChunkDescriptors::Type>(EChunkDescriptors::Type::CD_BIOME_TYPE_TOWN |
																		EChunkDescriptors::Type::CD_TWO_LANES | EChunkDescriptors::Type::CD_IS_MERGER);

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

}

// Called every frame
void ALevelChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


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
			}
		}

	}
	else
	{
		UE_LOG(LogLevelChunk, Error, TEXT("Chunk, %s, does not have a reference to the player!"), *GetName());
	}
}

//void ALevelChunk::refreshComponents()
//{
//	UActorComponent* comp = NewObject<UActorComponent>(this, USceneComponent::StaticClass(), "Howdy", RF_Transient);
//	check(comp);
//	comp->RegisterComponent();
//
//	AddComponent("HowdyScene", false, FTransform::Identity, this);
//
//	UE_LOG(LogLevelChunk, Error, TEXT("Chunk, %s, Howdy Called!"), *GetName());
//}

EChunkDescriptors::Type ALevelChunk::getChunkDescriptors() const
{
	return static_cast<EChunkDescriptors::Type>(_chunkDescriptors);
}

EChunkFeatures::Type ALevelChunk::getChunckFeatures() const
{
	return static_cast<EChunkFeatures::Type>(_chunkFeatures);
}

UStaticMeshComponent* ALevelChunk::getMesh() const
{
	return _mesh;
}

void ALevelChunk::setSpawnPoints(UPARAM(ref)TArray<USceneComponent*> carSpawnPoints, UPARAM(ref)TArray<USceneComponent*> obstableSpawnPoints, UPARAM(ref)TArray<USceneComponent*> pedestrianSpawnPoints)
{
	_carSpawnPoints = carSpawnPoints;
	_obstacleSpawnPoints = obstableSpawnPoints;
	_pedestrianSpawnPoints = pedestrianSpawnPoints;
}

const TArray<USceneComponent*>& ALevelChunk::getCarSpawnPoints() const
{
	return _carSpawnPoints;
}

const TArray<USceneComponent*>& ALevelChunk::getObstacleSpawnPoints() const
{
	return _obstacleSpawnPoints;
}

const TArray<USceneComponent*>& ALevelChunk::getPedestrianSpawnPoints() const
{
	return _pedestrianSpawnPoints;
}

