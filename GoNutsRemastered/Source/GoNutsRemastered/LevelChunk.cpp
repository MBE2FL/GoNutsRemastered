// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChunk.h"

const EChunkDescriptors::Type ALevelChunk::TOWN_THREE_LANES_ISLAND = static_cast<EChunkDescriptors::Type>(EChunkDescriptors::Type::CD_BIOME_TYPE_TOWN | 
																		EChunkDescriptors::Type::CD_THREE_LANES | EChunkDescriptors::Type::CD_LANE_CONTAINS_ISLAND);

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
	//_mesh->SetCollisionProfileName(TEXT("Custom"));
	//_mesh->SetCollisionObjectType(ECC_LevelSegmentChannel);
	//_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
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

}

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

