// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Components/ShapeComponent.h"
#include "MyShapeComponent.generated.h"

class FPrimitiveSceneProxy;

/**
 * 
 */
UCLASS(ClassGroup = "Level Chunks", hidecategories = (Object, LOD, Lighting, TextureStreaming), editinlinenew, meta = (DisplayName = "Lane Shape", BlueprintSpawnableComponent))
class GONUTSREMASTERED_API UMyShapeComponent : public UShapeComponent
{
	GENERATED_BODY()

public:
	UMyShapeComponent(const FObjectInitializer& ObjectInitializer);

	//########## USceneComponent Overrides ##########
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	//########## USceneComponent Overrides ##########

	// ########## UPrimitiveComponent Overrides ##########
	virtual bool IsZeroExtent() const override;
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual struct FCollisionShape GetCollisionShape(float Inflation = 0.0f) const override;
	// ########## UPrimitiveComponent Overrides ##########

	//########## UShapeComponent Overrides ##########
	virtual void UpdateBodySetup() override;
	//########## UShapeComponent Overrides ##########

	// @return the box dimensions, scaled by the component scale.
	UFUNCTION(BlueprintCallable, Category = "Components|Box")
	FVector2D GetScaledBoxDimensions() const;

	// @return the box dimensions, ignoring component scale.
	UFUNCTION(BlueprintCallable, Category = "Components|Box")
	FVector2D GetUnscaledBoxDimensions() const;
	
private:
	// The dimensions of the box. X is the full height, while Y is half the width.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lane|Shape", meta = (AllowPrivateAccess = true))
	FVector2D _boxDimensions;

	// Used for the line thickness when rendering.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lane|Shape", meta = (AllowPrivateAccess = true))
	float _lineThickness;
};
