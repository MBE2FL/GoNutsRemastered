// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "Engine/LevelStreaming.h"
#include "EngineUtils.h"
#include "LevelSegment.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	

	ALevelSegment* segment = nullptr;
	ALevelSegment* prevSegment = nullptr;
	TSubclassOf<ALevelSegment> classType;
	FVector vertPos = FVector::ZeroVector;

	for (int32 row = 0; row < _numRows; ++row)
	{
		for (int32 column = 0; column < _numColumns; ++column)
		{
			classType = _spawnableActors[FMath::RandRange(0, _spawnableActors.Num() - 1)];
			segment = Cast<ALevelSegment>(GetWorld()->SpawnActor(classType));


			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(segment->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			FVector halfBounds;
			if (mesh)
			{
				halfBounds = mesh->Bounds.BoxExtent;
				UE_LOG(LogTemp, Warning, TEXT("Bounds Extends: %s"), *halfBounds.ToString());
				UE_LOG(LogTemp, Warning, TEXT("Mesh Bounds: %s"), *mesh->GetStaticMesh()->GetBounds().BoxExtent.ToString());
			}


			FVector horPos;

			if (!prevSegment)
			{
				horPos = FVector(vertPos.X, 0.0f, 500.0f);
			}
			else
			{
				//TArray<UActorComponent*> comps = prevSegment->GetComponentsByClass(USceneComponent::StaticClass());
				//TInlineComponentArray<USceneComponent*> comps;
				//prevSegment->GetComponents(comps);
				TArray<UActorComponent*> comps = prevSegment->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("HSlot"));

				//UE_LOG(LogTemp, Warning, TEXT("Num Comps: %d"), comps.Num());

				if (comps.Num() != 1)
				{
					UE_LOG(LogTemp, Error, TEXT("Level Gen Actor does not have the required horizontal slot!"));
					continue;
				}
				else
				{
					horPos = Cast<USceneComponent>(comps[0])->GetComponentLocation();
				}
			}

			FTransform transform = FTransform(FRotator::ZeroRotator, FVector(vertPos.X + halfBounds.X, horPos.Y + halfBounds.Y, 500.0f), FVector(10.0f, 10.0f, 1.0f));
			segment->SetActorTransform(transform);

			prevSegment = segment;



			// Notify listeners for special segments.
			switch (segment->getSegmentType())
			{
			case ESegmentTypes::Generic:
				break;
			case ESegmentTypes::Crosswalk:
				_onCrosswalkSpawnedEvent.Broadcast(segment);
				break;
			default:
				break;
			}
		}


		//TInlineComponentArray<USceneComponent*> comps;
		//vertPos = comps[1]->GetComponentLocation();


		TArray<UActorComponent*> comps = prevSegment->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("VSlot"));

		if (comps.Num() != 1)
		{
			UE_LOG(LogTemp, Error, TEXT("Level Gen Actor does not have the required vertical slot!"));
			continue;
		}
		else
		{
			vertPos = Cast<USceneComponent>(comps[0])->GetComponentLocation();
		}

		prevSegment = nullptr;
	}
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelGenerator::streamLevelsTest()
{
	ULevelStreaming* levelStream = nullptr;
	ULevelStreaming* levelStreamInstance = nullptr;
	uint32 levelNum;

	for (int32 row = 0; row < _numRows; ++row)
	{
		for (int32 column = 0; column < _numColumns; ++column)
		{
			// Create an instance of a level stream.
			levelNum = FMath::RandRange(0, _levelStreamNames.Num() - 1);
			levelStream = UGameplayStatics::GetStreamingLevel(GetWorld(), _levelStreamNames[levelNum]);
			FString uniqueLevelStreamName = "LevelStream_";
			uniqueLevelStreamName.Appendf(TEXT("X%u_Y%u"), row, column);
			levelStreamInstance = levelStream->CreateInstance(uniqueLevelStreamName);

			// Get horizontal and vertical slots from the segment.
			//AActor* segment;
			//FVector hSlot;
			//FVector vSlot;
			//for (TActorIterator<AActor> it(GetWorld()); it; ++it)
			//{
			//	if (it->GetName() == uniqueLevelStreamName)
			//	{
			//		segment = *it;
			//		TArray<UActorComponent*> slotComps = segment->GetComponentsByClass(USceneComponent::StaticClass());

			//		if (slotComps[0]->ComponentHasTag(TEXT("HSlot")))
			//		{
			//			Cast<USceneComponent>(slotComps[0])->GetRelativeTransform().GetLocation();
			//		}

			//		break;
			//	}
			//}

			//ULevel* level = levelStreamInstance->GetLoadedLevel();

			//if (level)
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("Level Loaded!"));

			//	for (AActor* segment : level->Actors)
			//	{
			//		UE_LOG(LogTemp, Warning, TEXT("Segment: %s"), *segment->GetName());
			//	}
			//}
			//else
			//{
			//	UE_LOG(LogTemp, Error, TEXT("Level Not Loaded!"));
			//}

			//FLatentActionInfo actionInfo;
			//UGameplayStatics::LoadStreamLevel(GetWorld(), FName(uniqueLevelStreamName), true, true, actionInfo);

			UWorld* world = GetWorld();
			TArray<ULevelStreaming*> levelStreams = world->GetStreamingLevels();
			for (ULevelStreaming* stream : levelStreams)
			{
				ULevel* level = stream->GetLoadedLevel();

				if (level)
				{
					UE_LOG(LogTemp, Warning, TEXT("Level Loaded! %s"), *(level->GetName()));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Level Not Loaded! %s"), *(stream->GetName()));
				}
			}



			// Set the transform of the level stream.
			FVector pos = FVector(row * 1000.0f, column * 1000.0f, 0.0f);
			FTransform transform = FTransform(FRotator::ZeroRotator, pos, FVector::OneVector);
			levelStreamInstance->LevelTransform = transform;
			//levelStreamInstance->LevelTransform.TransformPosition(pos);

			levelStreamInstance->SetShouldBeLoaded(true);
			levelStreamInstance->SetShouldBeVisible(true);

			UE_LOG(LogTemp, Warning, TEXT("LevelStream_X%u_Y%u"), row, column);
		}
	}
}

