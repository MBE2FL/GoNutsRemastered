// Fill out your copyright notice in the Description page of Project Settings.


#include "LaneComponent.h"
#include "Obstacle.h"

DEFINE_LOG_CATEGORY(LogLane);

const uint8 ULaneComponent::MAX_OBSTACLES = 5;

// Sets default values for this component's properties
ULaneComponent::ULaneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	_obstacleTypes = static_cast<uint8>(EObstacleType::OT_NONE);


	BoxExtent = FVector(500.0f, 165.0f, 50.0f);

	static const FName collisionProfileName(TEXT("Custom"));
	SetCollisionProfileName(collisionProfileName);
	SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	SetCollisionObjectType(ECC_LaneChannel);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ShapeColor = FColor(255, 0, 0, 255);
}


// Called when the game starts
void ULaneComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	_obstacles.Reserve(MAX_OBSTACLES);
}

FPrimitiveSceneProxy* ULaneComponent::CreateSceneProxy()
{
	/** Represents a UBoxComponent to the scene manager. */
	class FBoxSceneProxy final : public FPrimitiveSceneProxy
	{
	public:
		SIZE_T GetTypeHash() const override
		{
			static size_t UniquePointer;
			return reinterpret_cast<size_t>(&UniquePointer);
		}

		FBoxSceneProxy(const UBoxComponent* InComponent)
			: FPrimitiveSceneProxy(InComponent)
			, bDrawOnlyIfSelected(InComponent->bDrawOnlyIfSelected)
			, BoxExtents(InComponent->GetUnscaledBoxExtent())
			, BoxColor(InComponent->ShapeColor)
			, LineThickness(0.0f)
		{
			bWillEverBeLit = true;
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

					const FLinearColor DrawColor = GetViewSelectionColor(BoxColor, *View, IsSelected(), IsHovered(), false, IsIndividuallySelected());

					FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);
					DrawOrientedWireBox(PDI, LocalToWorld.GetOrigin(), LocalToWorld.GetScaledAxis(EAxis::X), LocalToWorld.GetScaledAxis(EAxis::Y), LocalToWorld.GetScaledAxis(EAxis::Z), BoxExtents, DrawColor, SDPG_World, LineThickness);
				}
			}
		}

		virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
		{
			const bool bProxyVisible = !bDrawOnlyIfSelected || IsSelected();

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
		const uint32	bDrawOnlyIfSelected : 1;
		const FVector	BoxExtents;
		const FColor	BoxColor;
		const float LineThickness;
	};

	return new FBoxSceneProxy(this);
}

// Called every frame
void ULaneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

}

const TArray<AObstacle*>& ULaneComponent::getObstacles() const
{
	return _obstacles;
}

uint8 ULaneComponent::getObstacleTypes() const
{
	return _obstacleTypes;
}

void ULaneComponent::recycleObstacles()
{
	for (AObstacle* obstacle : _obstacles)
	{
		// TO-DO Recycle obstacle. For now just delete.
		obstacle->Destroy();
	}

	_obstacles.Empty();
}

void ULaneComponent::addObstacle(AObstacle* obstacle)
{
	_obstacles.Emplace(obstacle);
}

void ULaneComponent::addObstacles(const TArray<AObstacle*>& obstacles)
{
	_obstacles += obstacles;
}

bool ULaneComponent::isAltLane() const
{
	return _isAltLane;
}

ULaneComponent* ULaneComponent::getLeftLane() const
{
	return _leftLane;
}

ULaneComponent* ULaneComponent::getRightLane() const
{
	return _rightLane;
}

