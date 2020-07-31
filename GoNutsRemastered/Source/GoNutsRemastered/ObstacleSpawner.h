// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObstacleSpawner.generated.h"


class ALevelGenerator;
class AObstacle;
enum class EObstacleType : uint8;


USTRUCT(Blueprintable)
struct FObstacleClassTypes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle Spawner|Obstacle Types")
	TArray<TSubclassOf<AObstacle>> _obstacleClassTypes;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GONUTSREMASTERED_API UObstacleSpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObstacleSpawner();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	ALevelGenerator *_levelGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle Spawner|Obstacle Types", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<AActor>> _RoadObstacleTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle Spawner|Obstacle Types", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<AActor>> _GrassObstacleTypes;

	UFUNCTION()
	void spawnObstacle(ALevelChunk* road, const TArray<USceneComponent*>& ObstacleSpawnPoints);



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle Spawner|Obstacle Types", meta = (AllowPrivateAccess = true))
	TMap<EObstacleType, FObstacleClassTypes> _obstaclesTypes;

#if WITH_EDITOR
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle Spawner|Obstacle Types", meta = (AllowPrivateAccess = true))
	bool _refreshObstacleClassTypes = false;

	UFUNCTION()
	void getAllObstacleClassTypes();
#endif
};
