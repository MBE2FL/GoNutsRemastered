// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelGenerator.h"
#include "ObstacleSpawner.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GONUTSREMASTERED_API UObstacleSpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObstacleSpawner();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	ALevelGenerator *_levelGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle Spawner|Obstacle Types", meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<AActor>> _ObstacleTypes;

	UFUNCTION()
		void spawnObstacle(ALevelChunk* road, const TArray<USceneComponent*>& ObstacleSpawnPoints);
};
