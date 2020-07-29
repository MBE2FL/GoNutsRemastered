// Fill out your copyright notice in the Description page of Project Settings.


#include "LaneComponent.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif

const uint8 ULaneComponent::MAX_OBSTACLES = 5;

// Sets default values for this component's properties
ULaneComponent::ULaneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULaneComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	_obstacles.Reserve(MAX_OBSTACLES);
}


// Called every frame
void ULaneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

#if WITH_EDITOR
	//UE_LOG(LogTemp, Error, TEXT("Component ticking."));

	//if (!GWorld->HasBegunPlay())
	//UWorld* world = GEngine->GetWorldFromContextObject(this);
	UWorld* world = Cast<UObject>(this)->GetWorld();

	if (IsValid(world))
	{
		if (world->WorldType == EWorldType::Type::EditorPreview)
		{
			DrawDebugBox(world, GetRelativeLocation(), FVector(_boundingBox.GetSize(), 50.0f), FColor::Red, false, -1.0f, 0, 4.0f);

			return;
		}
	}
#endif
}

