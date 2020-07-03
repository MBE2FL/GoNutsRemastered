// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSegment.generated.h"


class UArrowComponent;

#define ECC_LevelSegmentChannel ECollisionChannel::ECC_GameTraceChannel1


UENUM(meta = (Bitflags))
enum class ESegmentTypes : uint8
{
	None,
	Generic,
	Crosswalk,
	House,
	Road,
	Sidewalk,
};
ENUM_CLASS_FLAGS(ESegmentTypes);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESegmentOrientations : uint8
{
	SEGO_None = 0	UMETA(DisplayName = "None (DON'T USE THIS!)"),
	SEGO_Right = 1	UMETA(DisplayName = "Right"),
	SEGO_Left = 2	UMETA(DisplayName = "Left"),
	SEGO_Up = 4		UMETA(DisplayName = "Up"),
	SEGO_Down = 8	UMETA(DisplayName = "Down"),
};
//ENUM_CLASS_FLAGS(ESegmentOrientations);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
namespace ESegmentFeatures
{
	enum Type
	{
		SF_None = 0	UMETA(DisplayName = "None (DON'T USE THIS!)"),
		SF_No_Features = 1	UMETA(DisplayName = "No Features"),
		SF_Spawn_Cars = 2	UMETA(DisplayName = "Spawn Cars"),
		SF_Spawn_Obstacles = 4	UMETA(DisplayName = "Spawn Obstacles"),
		SF_Spawn_Pedestrians = 8	UMETA(DisplayName = "Spawn Pedestrians"),
	};
}

USTRUCT(BlueprintType)
struct FSegmentSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	TSubclassOf<ALevelSegment> _segment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment|Spawnable Actor Settings", meta = (Bitmask, BitmaskEnum = "ESegmentOrientations", AllowPrivateAccess = true))
	uint8 _validOrientations; // TO-DO Test down, may not fit into an 8-bit integer.

	bool operator==(const FSegmentSpawnInfo& other) const
	{
		//FString name = FText::FromName(_segment->GetFName()).ToString();
		//FString name2 = FText::FromName(other._segment->GetFName()).ToString();
		//bool segment = (_segment == other._segment);
		//bool ori = (_validOrientations & other._validOrientations);
		//UE_LOG(LogTemp, Warning, TEXT("SegmentSpawnInfo: Segment_1: %s"), *name);
		//UE_LOG(LogTemp, Warning, TEXT("SegmentSpawnInfo: Segment_2: %s"), *name2);
		//UE_LOG(LogTemp, Warning, TEXT("SegmentSpawnInfo: Segment: %i, Orientation: %i"), static_cast<int32>(segment), static_cast<int32>(ori));

		return (_validOrientations & other._validOrientations) && (_segment == other._segment);
	}
};

FORCEINLINE uint32 GetTypeHash(const FSegmentSpawnInfo& segmentSpawnInfo)
{
	return FCrc::MemCrc_DEPRECATED(&segmentSpawnInfo, sizeof(segmentSpawnInfo));
}



USTRUCT(BlueprintType)
struct FSegmentGroup
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	TSet<TSubclassOf<ALevelSegment>> _validRightSegments;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	TSet<TSubclassOf<ALevelSegment>> _validLeftSegments;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	TSet<TSubclassOf<ALevelSegment>> _validUpSegments;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Gen|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	TSet<TSubclassOf<ALevelSegment>> _validDownSegments;
};


UCLASS(Blueprintable)
class GONUTSREMASTERED_API ALevelSegment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelSegment();

	ESegmentTypes getSegmentType() const;

	ESegmentFeatures::Type getSegmentFeatures() const;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;

	UStaticMeshComponent* getMesh() const;

	UFUNCTION(BlueprintCallable, Category = "Level Segment")
	float getHOffset() const;

	UFUNCTION(BlueprintCallable, Category = "Level Segment")
	FVector getHOffsetLocation() const;

	ESegmentOrientations getOrientation() const;

	void setOrientation(const ESegmentOrientations orientation);

	UFUNCTION(BlueprintCallable, Category = "Level Segment|Segment Features")
	void setSpawnPoints(UPARAM(ref) TArray<USceneComponent*> carSpawnPoints, UPARAM(ref) TArray<USceneComponent*> obstableSpawnPoints,
						UPARAM(ref) TArray<USceneComponent*> pedestrianSpawnPoints);

	UFUNCTION(BlueprintCallable, Category = "Level Segment|Segment Features")
	const TArray<USceneComponent*>& getObstacleSpawnPoints() const;

	UFUNCTION(BlueprintCallable, Category = "Level Segment|Segment Features")
	const TArray<USceneComponent*>& getPedestrianSpawnPoints() const;


	const TSet<FSegmentSpawnInfo>& getValidRightSegments() const;
	const TSet<FSegmentSpawnInfo>& getValidLeftSegments() const;
	const TSet<FSegmentSpawnInfo>& getValidUpSegments() const;
	const TSet<FSegmentSpawnInfo>& getValidDownSegments() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment", meta = (AllowPrivateAccess = true))
	ESegmentTypes _segmentType = ESegmentTypes::Generic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment", meta = (Bitmask, BitmaskEnum = "ESegmentFeatures"))
	int32 _segmentFeatures = ESegmentFeatures::Type::SF_No_Features;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment|Connection Settings", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* _mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment", meta = (AllowPrivateAccess = true))
	UArrowComponent* _rightDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment", meta = (AllowPrivateAccess = true))
	UArrowComponent* _leftDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment", meta = (AllowPrivateAccess = true))
	UArrowComponent* _topDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment", meta = (AllowPrivateAccess = true))
	UArrowComponent* _bottomDir;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level Segment", meta = (AllowPrivateAccess = true))
	ESegmentOrientations _orientation = ESegmentOrientations::SEGO_Right;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	TSet<FSegmentSpawnInfo> _validRightSegments;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	TSet<FSegmentSpawnInfo> _validLeftSegments;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	TSet<FSegmentSpawnInfo> _validTopSegments;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	TSet<FSegmentSpawnInfo> _validBottomSegments;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment|Spawnable Actor Settings", meta = (AllowPrivateAccess = true))
	//TMap<ESegmentTypes, FSegmentSpawnInfo> _validOrientations;


	UPROPERTY()
		TArray<USceneComponent*> _carSpawnPoints;
	UPROPERTY()
		TArray<USceneComponent*> _obstacleSpawnPoints;
	UPROPERTY()
		TArray<USceneComponent*> _pedestrianSpawnPoints;
};
