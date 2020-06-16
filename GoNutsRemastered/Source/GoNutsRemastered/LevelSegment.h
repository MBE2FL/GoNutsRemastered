// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSegment.generated.h"

UENUM(BlueprintType)
enum class ESegmentTypes : uint8
{
	Generic,
	Crosswalk
};

UCLASS()
class GONUTSREMASTERED_API ALevelSegment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelSegment();

	ESegmentTypes getSegmentType() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Segment", meta = (AllowPrivateAccess = true))
	ESegmentTypes _segmentType = ESegmentTypes::Generic;
};
