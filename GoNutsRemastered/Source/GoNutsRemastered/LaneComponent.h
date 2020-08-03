// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LaneComponent.generated.h"


class AObstacle;


DECLARE_LOG_CATEGORY_EXTERN(LogLane, Log, All);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GONUTSREMASTERED_API ULaneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULaneComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	uint8 getObstacleTypes() const;


	static const uint8 MAX_OBSTACLES;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lane", meta = (AllowPrivateAccess = true))
	TArray<AObstacle*> _obstacles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane", meta = (Bitmask, BitmaskEnum = "EObstacleType", AllowPrivateAccess = true))
	uint8 _obstacleTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane", meta = (AllowPrivateAccess = true))
	FVector2D _boundingBox;
};
