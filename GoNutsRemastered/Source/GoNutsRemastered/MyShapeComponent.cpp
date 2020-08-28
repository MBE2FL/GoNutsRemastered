// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShapeComponent.h"
#include "WorldCollision.h"
#include "PrimitiveViewRelevance.h"
#include "PrimitiveSceneProxy.h"
#include "SceneManagement.h"
#include "PhysicsEngine/BoxElem.h"
#include "PhysicsEngine/BodySetup.h"


UMyShapeComponent::UMyShapeComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	_boxDimensions = FVector2D(32.0f, 32.0f);

	bUseEditorCompositing = true;
}

FBoxSphereBounds UMyShapeComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FVector extends = FVector(_boxDimensions.X * 0.5f, _boxDimensions.Y, 50.0f);
	return FBoxSphereBounds(FBox(-extends, extends)).TransformBy(LocalToWorld);
}

bool UMyShapeComponent::IsZeroExtent() const
{
	return _boxDimensions.IsZero();
}

FPrimitiveSceneProxy* UMyShapeComponent::CreateSceneProxy()
{
	/** Represents a UMyShapeComponent to the scene manager. */
	class FLaneSceneProxy final : public FPrimitiveSceneProxy
	{
	public:
		SIZE_T GetTypeHash() const override
		{
			static size_t UniquePointer;
			return reinterpret_cast<size_t>(&UniquePointer);
		}

		FLaneSceneProxy(const UMyShapeComponent* InComponent)
			: FPrimitiveSceneProxy(InComponent)
			, _drawOnlyIfSelected(InComponent->bDrawOnlyIfSelected)
			, _boxDimensions(InComponent->GetUnscaledBoxDimensions())
			, _boxColor(InComponent->ShapeColor)
			, _lineThickness(InComponent->_lineThickness)
		{
			bWillEverBeLit = false;
		}

		virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
		{
			QUICK_SCOPE_CYCLE_COUNTER(STAT_BoxSceneProxy_GetDynamicMeshElements);

			const FMatrix& LocalToWorld = GetLocalToWorld();

			for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
			{
				if (VisibilityMap & (1 << ViewIndex))
				{
					const FSceneView* View = Views[ViewIndex];

					const FLinearColor DrawColor = GetViewSelectionColor(_boxColor, *View, IsSelected(), IsHovered(), false, IsIndividuallySelected());

					FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);
					DrawOrientedWireBox(PDI, LocalToWorld.GetOrigin(), 
						LocalToWorld.GetScaledAxis(EAxis::X), 
						LocalToWorld.GetScaledAxis(EAxis::Y), 
						LocalToWorld.GetScaledAxis(EAxis::Z), 
						FVector(_boxDimensions, 50.0f), 
						DrawColor, SDPG_World, _lineThickness);
				}
			}
		}

		virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
		{
			const bool bProxyVisible = !_drawOnlyIfSelected || IsSelected();

			// Should we draw this because collision drawing is enabled, and we have collision
			const bool bShowForCollision = View->Family->EngineShowFlags.Collision && IsCollisionEnabled();

			FPrimitiveViewRelevance Result;
			Result.bDrawRelevance = (IsShown(View) && bProxyVisible) || bShowForCollision;
			Result.bDynamicRelevance = true;
			Result.bShadowRelevance = IsShadowCast(View);
			Result.bEditorPrimitiveRelevance = UseEditorCompositing(View);
			return Result;
		}
		virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }
		uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

	private:
		const uint32	_drawOnlyIfSelected : 1;
		const FVector2D	_boxDimensions;
		const FColor	_boxColor;
		const float _lineThickness;
	};

	return new FLaneSceneProxy(this);
}

FCollisionShape UMyShapeComponent::GetCollisionShape(float Inflation) const
{
	FVector Extent = FVector(GetScaledBoxDimensions(), 50.0f) + Inflation;
	if (Inflation < 0.f)
	{
		// Don't shrink below zero size.
		Extent = Extent.ComponentMax(FVector::ZeroVector);
	}

	return FCollisionShape::MakeBox(Extent);
}

template <EShapeBodySetupHelper UpdateBodySetupAction>
bool InvalidateOrUpdateBoxBodySetup(UBodySetup*& ShapeBodySetup, bool bUseArchetypeBodySetup, FVector BoxExtent)
{
	check((bUseArchetypeBodySetup && UpdateBodySetupAction == EShapeBodySetupHelper::InvalidateSharingIfStale) || (!bUseArchetypeBodySetup && UpdateBodySetupAction == EShapeBodySetupHelper::UpdateBodySetup));
	check(ShapeBodySetup->AggGeom.BoxElems.Num() == 1);
	FKBoxElem* se = ShapeBodySetup->AggGeom.BoxElems.GetData();

	// @todo UE4 do we allow this now?
	// check for malformed values
	if (BoxExtent.X < KINDA_SMALL_NUMBER)
	{
		BoxExtent.X = 1.0f;
	}

	if (BoxExtent.Y < KINDA_SMALL_NUMBER)
	{
		BoxExtent.Y = 1.0f;
	}

	if (BoxExtent.Z < KINDA_SMALL_NUMBER)
	{
		BoxExtent.Z = 1.0f;
	}

	float XExtent = BoxExtent.X * 2.f;
	float YExtent = BoxExtent.Y * 2.f;
	float ZExtent = BoxExtent.Z * 2.f;

	if (UpdateBodySetupAction == EShapeBodySetupHelper::UpdateBodySetup)
	{
		// now set the PhysX data values
		se->SetTransform(FTransform::Identity);
		se->X = XExtent;
		se->Y = YExtent;
		se->Z = ZExtent;
	}
	else if (se->X != XExtent || se->Y != YExtent || se->Z != ZExtent)
	{
		ShapeBodySetup = nullptr;
		bUseArchetypeBodySetup = false;
	}

	return bUseArchetypeBodySetup;
}

void UMyShapeComponent::UpdateBodySetup()
{
	//FVector extends = FVector(GetUnscaledBoxDimensions(), 50.0f);

	//if (PrepareSharedBodySetup<UMyShapeComponent>())
	//{
	//	bUseArchetypeBodySetup = InvalidateOrUpdateBoxBodySetup<EShapeBodySetupHelper::InvalidateSharingIfStale>(ShapeBodySetup, bUseArchetypeBodySetup, extends);
	//}

	//CreateShapeBodySetupIfNeeded<FKBoxElem>();

	//if (!bUseArchetypeBodySetup)
	//{
	//	InvalidateOrUpdateBoxBodySetup<EShapeBodySetupHelper::UpdateBodySetup>(ShapeBodySetup, bUseArchetypeBodySetup, extends);
	//}
}

FVector2D UMyShapeComponent::GetScaledBoxDimensions() const
{
	return _boxDimensions * FVector2D(GetComponentTransform().GetScale3D());
}

FVector2D UMyShapeComponent::GetUnscaledBoxDimensions() const
{
	return _boxDimensions;
}
