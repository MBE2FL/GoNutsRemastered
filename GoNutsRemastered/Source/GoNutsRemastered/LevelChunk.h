// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelChunk.generated.h"


class ULaneComponent;


DECLARE_LOG_CATEGORY_EXTERN(LogLevelChunk, Log, All);


//UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
//namespace EChunkDescriptors
//{
//	enum Type
//	{
//		CD_NONE = 0	UMETA(DisplayName = "None (DON'T USE THIS!)"),
//
//		CD_BIOME_TYPE_TOWN = 1	UMETA(DisplayName = "Biome Type Town"),
//		CD_BIOME_TYPE_CITY = 2	UMETA(DisplayName = "Biome Type City"),
//		CD_BIOME_TYPE_FOREST = 4	UMETA(DisplayName = "Biome Type Forest"),
//
//		CD_LANE_CONTAINS_ISLAND = 8	UMETA(DisplayName = "Lane Contains Island"),
//
//		CD_TWO_LANES = 16	UMETA(DisplayName = "2 Lanes"),
//		CD_THREE_LANES = 32	UMETA(DisplayName = "3 Lanes"),
//		CD_FOUR_LANES = 64	UMETA(DisplayName = "4 Lanes"),
//
//		CD_IS_INTERSECTION = 128	UMETA(DisplayName = "Is An Intersection"),
//
//		CD_IS_MERGER = 256	UMETA(DisplayName = "Is A Merger"),
//	};
//}

UENUM(BlueprintType)
enum class EChunkDescriptors : uint8
{
	CD_NONE = 0	UMETA(DisplayName = "None (DON'T USE THIS!)"),

	CD_TOWN_THREE_LANES_ISLAND			UMETA(DisplayName = "Town, Three Lanes With Island"),
	CD_TOWN_THREE_LANES_INTERSECTION	UMETA(DisplayName = "Town, Three Lanes, Intersection"),
	CD_TOWN_TWO_LANES					UMETA(DisplayName = "Town, Two Lanes"),
	CD_TOWN_THREE_TO_TWO_LANES_MERGER	UMETA(DisplayName = "Town, Three To Two Lanes, Merger"),
};


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

		CF_PLAYER_TURN_LEFT = 16	UMETA(DisplayName = "Player Can Turn Left"),
		CF_PLAYER_TURN_RIGHT = 32	UMETA(DisplayName = "Player Can Turn Right"),
		CF_PLAYER_TURN_LEFT_RIGHT = CF_PLAYER_TURN_LEFT | CF_PLAYER_TURN_RIGHT	UMETA(DisplayName = "Player Can Turn Left Or Right")
	};
}


#define ECC_LevelChunkChannel ECollisionChannel::ECC_GameTraceChannel1


UCLASS()
class GONUTSREMASTERED_API ALevelChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual bool ShouldTickIfViewportsOnly() const override { return true; };
#endif
	
	UFUNCTION(BlueprintCallable, Category = "Chunk|Chunk Features")
	EChunkDescriptors getChunkDescriptor() const;

	UFUNCTION(BlueprintCallable, Category = "Chunk|Chunk Features")
	EChunkFeatures::Type getChunckFeatures() const;

	UFUNCTION(BlueprintCallable, Category = "Chunk|Chunk Features")
	UStaticMeshComponent* getMesh() const;

	UFUNCTION(BlueprintCallable, Category = "Chunk|Lanes")
	const TArray<ULaneComponent*>& getLanes() const;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Connection Settings", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* _mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk", meta = (AllowPrivateAccess = true))
	EChunkDescriptors _chunkDescriptor = EChunkDescriptors::CD_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk", meta = (Bitmask, BitmaskEnum = "EChunkFeatures"), meta = (AllowPrivateAccess = true))
	int32 _chunkFeatures = EChunkFeatures::Type::CF_NONE;


	UPROPERTY()
	TArray<ULaneComponent*> _lanes;
};
