// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelChunk.generated.h"



UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
namespace EChunkDescriptors
{
	enum Type
	{
		CD_NONE = 0	UMETA(DisplayName = "None (DON'T USE THIS!)"),

		CD_BIOME_TYPE_TOWN = 1	UMETA(DisplayName = "Biome Type Town"),
		CD_BIOME_TYPE_CITY = 2	UMETA(DisplayName = "Biome Type City"),
		CD_BIOME_TYPE_FOREST = 4	UMETA(DisplayName = "Biome Type Forest"),

		CD_LANE_CONTAINS_ISLAND = 8	UMETA(DisplayName = "Lane Contains Island"),

		CD_TWO_LANES = 16	UMETA(DisplayName = "2 Lanes"),
		CD_THREE_LANES = 32	UMETA(DisplayName = "3 Lanes"),
		CD_FOUR_LANES = 64	UMETA(DisplayName = "4 Lanes"),

		CD_IS_INTERSECTION = 128	UMETA(DisplayName = "Is An Intersection"),

		CD_IS_MERGER = 256	UMETA(DisplayName = "Is A Merger"),
	};
}


UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
namespace EChunkFeatures
{
	enum Type
	{
		CF_NONE = 0	UMETA(DisplayName = "None (DON'T USE THIS!)"),
		CF_NO_FEATURES = 1	UMETA(DisplayName = "No Features"),
		CF_SPAWN_CARS = 2	UMETA(DisplayName = "Spawn Cars"),
		CF_SPAWN_OBSTACLES = 4	UMETA(DisplayName = "Spawn Obstacles"),
		CF_SPAWN_PEDESTRIANS = 8	UMETA(DisplayName = "Spawn Pedestrians"),
	};
}



UCLASS()
class GONUTSREMASTERED_API ALevelChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelChunk();

	static const EChunkDescriptors::Type TOWN_THREE_LANES_ISLAND;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "Chunk|Chunk Features")
	EChunkDescriptors::Type getChunkDescriptors() const;

	UFUNCTION(BlueprintCallable, Category = "Chunk|Chunk Features")
	EChunkFeatures::Type getChunckFeatures() const;

	UFUNCTION(BlueprintCallable, Category = "Chunk|Chunk Features")
	UStaticMeshComponent* getMesh() const;

	UFUNCTION(BlueprintCallable, Category = "Chunk|Chunk Features")
	void setSpawnPoints(UPARAM(ref) TArray<USceneComponent*> carSpawnPoints, UPARAM(ref) TArray<USceneComponent*> obstableSpawnPoints,
			UPARAM(ref) TArray<USceneComponent*> pedestrianSpawnPoints);

	UFUNCTION(BlueprintCallable, Category = "Chunk|Chunk Features")
	const TArray<USceneComponent*>& getCarSpawnPoints() const;

	UFUNCTION(BlueprintCallable, Category = "Chunk|Chunk Features")
	const TArray<USceneComponent*>& getObstacleSpawnPoints() const;

	UFUNCTION(BlueprintCallable, Category = "Chunk|Chunk Features")
	const TArray<USceneComponent*>& getPedestrianSpawnPoints() const;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Connection Settings", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* _mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk", meta = (Bitmask, BitmaskEnum = "EChunkDescriptors"), meta = (AllowPrivateAccess = true))
	int32 _chunkDescriptors = EChunkDescriptors::Type::CD_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk", meta = (Bitmask, BitmaskEnum = "EChunkFeatures"), meta = (AllowPrivateAccess = true))
	int32 _chunkFeatures = EChunkFeatures::Type::CF_NONE;

	UPROPERTY()
	TArray<USceneComponent*> _carSpawnPoints;
	UPROPERTY()
	TArray<USceneComponent*> _obstacleSpawnPoints;
	UPROPERTY()
	TArray<USceneComponent*> _pedestrianSpawnPoints;
};
