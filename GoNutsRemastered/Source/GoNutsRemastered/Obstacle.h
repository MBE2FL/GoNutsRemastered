// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EObstacleType : uint8
{
	OT_NONE = 0	UMETA(DisplayName = "None (DON'T USE THIS!)"),
	OT_ALL_TERRAIN_OBSTACLE = 1	UMETA(DisplayName = "All Terrain Obstacle"),
	OT_GRASS_OBSTACLE = 2	UMETA(DisplayName = "Grass Obstacle"),
	OT_ROAD_OBSTACLE = 4	UMETA(DisplayName = "Road Obstacle"),

};

UCLASS()
class GONUTSREMASTERED_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		EObstacleType getObstacleType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Connection Settings", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* _mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle", meta = (Bitmask, BitmaskEnum = "EObstacleType"), meta = (AllowPrivateAccess = true))
		uint8 ObstacleType = static_cast<uint8>(EObstacleType::OT_NONE);

private:	


};
