// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	_mesh->SetStaticMesh(meshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> matAsset(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	_mesh->SetMaterial(0, matAsset.Object);
	_mesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	_mesh->SetCollisionObjectType(ECC_Pawn);
	_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = _mesh;
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EObstacleType AObstacle::getObstacleType()
{
	return static_cast<EObstacleType>(ObstacleType);
}

