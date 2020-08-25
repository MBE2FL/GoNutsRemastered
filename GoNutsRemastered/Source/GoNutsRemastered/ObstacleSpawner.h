// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObstacleSpawner.generated.h"


class ALevelGenerator;
class AObstacle;
enum class EObstacleType : uint8;


DECLARE_LOG_CATEGORY_EXTERN(LogObstacleSpawner, Log, All);


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

	UFUNCTION()
		const TMap<EObstacleType, FObstacleClassTypes>& getObstacleClassType() const { return _obstaclesTypes; };

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
	void spawnObstacle(ALevelChunk* road);



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle Spawner|Obstacle Types", meta = (AllowPrivateAccess = true))
	TMap<EObstacleType, FObstacleClassTypes> _obstaclesTypes;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle Spawner|Obstacle Types", meta = (AllowPrivateAccess = true))
	bool _refreshObstacleClassTypes = false;
#endif

#if WITH_EDITOR
	UFUNCTION()
	void getAllObstacleClassTypes();
#endif
};
