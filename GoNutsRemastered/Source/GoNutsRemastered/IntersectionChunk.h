// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelChunk.h"
#include "IntersectionChunk.generated.h"

/**
 * 
 */
UCLASS()
class GONUTSREMASTERED_API AIntersectionChunk : public ALevelChunk
{
	GENERATED_BODY()

public:
	AIntersectionChunk();
	USceneComponent* getLeftConnector() const;
	USceneComponent* getRightConnector() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Intersection Settings", meta = (AllowPrivateAccess = true))
	USceneComponent* _leftConnector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk|Intersection Settings", meta = (AllowPrivateAccess = true))
	USceneComponent* _rightConnector;
};
