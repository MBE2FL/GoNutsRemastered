// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LaneComponent.generated.h"


//class AObstacle;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GONUTSREMASTERED_API ULaneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULaneComponent();

	//virtual void OnRegister() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	static const uint8 MAX_OBSTACLES;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lane", meta = (AllowPrivateAccess = true))
	TArray<AActor*> _obstacles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane", meta = (AllowPrivateAccess = true))
	FBox2D _boundingBox;
};
