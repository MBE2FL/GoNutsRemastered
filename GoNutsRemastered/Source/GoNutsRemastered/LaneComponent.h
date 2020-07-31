// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LaneComponent.generated.h"


class AObstacle;


DECLARE_LOG_CATEGORY_EXTERN(LogLane, Log, All);


//#if WITH_EDITOR
//UCLASS(collapsecategories, hidecategories = Object, editinlinenew, MinimalAPI)
//class ULaneVisualizationComponent : public UPrimitiveComponent
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane|Lane Visualization", meta = (AllowPrivateAccess = true))
//	FBox2D _boundingBox;
//
//	//~ Begin UPrimitiveComponent Interface.
//	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
//	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
//	//~ End UPrimitiveComponent Interface.
//
//#if WITH_EDITOR
//	virtual bool IgnoreBoundsForEditorFocus() const override { return true; }
//#endif
//};
//#endif


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

//#if WITH_EDITOR
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane", meta = (AllowPrivateAccess = true))
//	ULaneVisualizationComponent* _visComp;
//#endif
};
