// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "Engine/LevelStreaming.h"
#include "EngineUtils.h"
#include "LevelSegment.h"

#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogLevelGen);

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


	// Create a group of valid segments for each segment.
	//for (TSubclassOf<ALevelSegment> segment : _spawnableActors)
	//{
	//	
	//}
	

	ALevelSegment* segment = nullptr;
	ALevelSegment* prevHSegment = nullptr;
	TSubclassOf<ALevelSegment> classType;

	for (int32 row = 0; row < _numRows; ++row)
	{
		for (int32 column = 0; column < _numColumns; ++column)
		{
			// Spawn a valid segment.
			//segment = getValidSegment(prevHSegment, prevVSegment);+
			UE_LOG(LogLevelGen, Warning, TEXT(" "));
			UE_LOG(LogLevelGen, Warning, TEXT("Row_%i, Column_%i"), row, column);
			segment = getValidSegment(prevHSegment);

			// Check for segment spawn failure.
			if (!segment)
			{
				UE_LOG(LogLevelGen, Error, TEXT("Segment failed to spawn!"));
				continue;
			}

			UE_LOG(LogLevelGen, Warning, TEXT("Segment %s spawned."), *FText::FromName(segment->GetFName()).ToString());


			// Notify listeners for special segments.
			switch (segment->getSegmentType())
			{
			case ESegmentTypes::Generic:
				break;
			case ESegmentTypes::Crosswalk:
				if (segment->getSegmentFeatures() & ESegmentFeatures::SF_Spawn_Pedestrians)
				{
					_onCrosswalkSpawnedEvent.Broadcast(segment, segment->getPedestrianSpawnPoints());
				}
				break;
			case ESegmentTypes::Road:
				if (segment->getSegmentFeatures() & ESegmentFeatures::SF_Spawn_Cars)
				{
					_onRoadSpawnedEvent.Broadcast(segment, segment->getObstacleSpawnPoints());
				}
				break;
			default:
				break;
			}


			// Store beginning of each row.
			if (column == 0)
			{
				_prevRowStart = segment;
			}

			// Store previous horizontal segment.
			prevHSegment = segment;
		}


		// Reset for next row.
		prevHSegment = nullptr;

		UE_LOG(LogLevelGen, Warning, TEXT("H_Segment Reset"));
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
			//	UE_LOG(LogLevelGen, Warning, TEXT("Level Loaded!"));

			//	for (AActor* segment : level->Actors)
			//	{
			//		UE_LOG(LogLevelGen, Warning, TEXT("Segment: %s"), *segment->GetName());
			//	}
			//}
			//else
			//{
			//	UE_LOG(LogLevelGen, Error, TEXT("Level Not Loaded!"));
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
					UE_LOG(LogLevelGen, Warning, TEXT("Level Loaded! %s"), *(level->GetName()));
				}
				else
				{
					UE_LOG(LogLevelGen, Error, TEXT("Level Not Loaded! %s"), *(stream->GetName()));
				}
			}



			// Set the transform of the level stream.
			FVector pos = FVector(row * 1000.0f, column * 1000.0f, 0.0f);
			FTransform transform = FTransform(FRotator::ZeroRotator, pos, FVector::OneVector);
			levelStreamInstance->LevelTransform = transform;
			//levelStreamInstance->LevelTransform.TransformPosition(pos);

			levelStreamInstance->SetShouldBeLoaded(true);
			levelStreamInstance->SetShouldBeVisible(true);

			UE_LOG(LogLevelGen, Warning, TEXT("LevelStream_X%u_Y%u"), row, column);
		}
	}
}


ALevelSegment* ALevelGenerator::getValidSegment(ALevelSegment* leftSegment, ALevelSegment* bottomSegment)
{
	ALevelSegment* segment = nullptr;

	ESegmentOrientations leftOrientation = ESegmentOrientations::SEGO_Right;
	ESegmentOrientations bottomOrientation = ESegmentOrientations::SEGO_Up;

	float vertPos = 0.0f;
	float horPos = 0.0f;


	// Most likely the beginning of the grid.
	if (!leftSegment && !bottomSegment)
	{
		FSegmentGroup* segmentGroup = _segmentGroups.Find(ESegmentTypes::Generic);

		if (!segmentGroup)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Failed to find valid segment group from the generic segment group."));
			return segment;
		}

		// Randomly choose a valid segment, and spawn it.
		TSubclassOf<ALevelSegment> classType = segmentGroup->_validRightSegments.Array()[FMath::RandRange(0, segmentGroup->_validRightSegments.Num() - 1)];
		segment = Cast<ALevelSegment>(GetWorld()->SpawnActor(classType));
		segment->SetActorLocation(FVector(vertPos, horPos, 500.0f));

		return segment;
	}
	// Most likely the beginning of a new row.
	else if (!leftSegment)
	{
		bottomOrientation = bottomSegment->getOrientation();

		TSet<FSegmentSpawnInfo> validSegments;

		// Get valid segments from the bottom segment.
		switch (bottomOrientation)
		{
		case ESegmentOrientations::SEGO_Right:
			validSegments = bottomSegment->getValidUpSegments();
			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has RIGHT orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Left:
			validSegments = bottomSegment->getValidDownSegments();
			vertPos = bottomSegment->GetActorLocation().X;

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has LEFT orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Up:
			validSegments = bottomSegment->getValidRightSegments();
			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has UP orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Down:
			validSegments = bottomSegment->getValidLeftSegments();
			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has DOWN orientation flag set!"), *bottomSegment->GetName());
			break;
		default:
			UE_LOG(LogLevelGen, Error, TEXT("BOTTOM segment, %s, had an invalid orientation."), *bottomSegment->GetName());
			break;
		}


		// Randomly choose a valid segment, and spawn it.
		FSegmentSpawnInfo segSpawnInfo = validSegments.Array()[FMath::RandRange(0, validSegments.Num() - 1)];
		segment = Cast<ALevelSegment>(GetWorld()->SpawnActor(segSpawnInfo._segment));

		// Set the position and rotation of the segment.
		//vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);
		segment->SetActorLocation(FVector(vertPos, horPos, 500.0f));
		setValidOrientation(segment, segSpawnInfo._validOrientations);

		return segment;
	}
	// Most likely a column on the first row.
	else if (!bottomSegment)
	{
		leftOrientation = leftSegment->getOrientation();

		TSet<FSegmentSpawnInfo> validSegments;

		// Get valid segments from the left segment.
		switch (leftOrientation)
		{
		case ESegmentOrientations::SEGO_Right:
			validSegments = leftSegment->getValidRightSegments();
			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has RIGHT orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Left:
			validSegments = leftSegment->getValidLeftSegments();
			horPos = leftSegment->GetActorLocation().Y;

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has LEFT orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Up:
			validSegments = leftSegment->getValidUpSegments();
			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has UP orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Down:
			validSegments = leftSegment->getValidDownSegments();
			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has DOWN orientation flag set!"), *leftSegment->GetName());
			break;
		default:
			UE_LOG(LogLevelGen, Error, TEXT("LEFT segment, %s, had an invalid orientation."), *leftSegment->GetName());
			break;
		}


		// Randomly choose a valid segment, and spawn it.
		FSegmentSpawnInfo segSpawnInfo = validSegments.Array()[FMath::RandRange(0, validSegments.Num() - 1)];
		segment = Cast<ALevelSegment>(GetWorld()->SpawnActor(segSpawnInfo._segment));

		// Set the position and rotation of the segment.
		//horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
		segment->SetActorLocation(FVector(vertPos, horPos, 500.0f));
		setValidOrientation(segment, segSpawnInfo._validOrientations);

		return segment;
	}
	// Both a left and bottom segment exist.
	else
	{
		leftOrientation = leftSegment->getOrientation();
		bottomOrientation = bottomSegment->getOrientation();


		// Find the segments valid for both segment types.
		//TSet<TSubclassOf<ALevelSegment>> validSegments;
		TSet<FSegmentSpawnInfo> validSegments;

		// Get valid segments from the left segment.
		switch (leftOrientation)
		{
		case ESegmentOrientations::SEGO_Right:
			validSegments = leftSegment->getValidRightSegments();
			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has RIGHT orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Left:
			validSegments = leftSegment->getValidLeftSegments();
			horPos = leftSegment->GetActorLocation().Y;

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has LEFT orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Up:
			validSegments = leftSegment->getValidUpSegments();
			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has UP orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Down:
			validSegments = leftSegment->getValidDownSegments();
			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has DOWN orientation flag set!"), *leftSegment->GetName());
			break;
		default:
			UE_LOG(LogLevelGen, Error, TEXT("LEFT segment, %s, had an invalid orientation."), *leftSegment->GetName());
			return segment;
			break;
		}

		// Get valid segments from the bottom segment.
		switch (bottomOrientation)
		{
		case ESegmentOrientations::SEGO_Right:
			validSegments = validSegments.Intersect(bottomSegment->getValidUpSegments());
			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has RIGHT orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Left:
			validSegments = validSegments.Intersect(bottomSegment->getValidDownSegments());
			vertPos = bottomSegment->GetActorLocation().X;

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has LEFT orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Up:
			validSegments = validSegments.Intersect(bottomSegment->getValidRightSegments());
			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has UP orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Down:
			validSegments = validSegments.Intersect(bottomSegment->getValidLeftSegments());
			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has DOWN orientation flag set!"), *bottomSegment->GetName());
			break;
		default:
			UE_LOG(LogLevelGen, Error, TEXT("BOTTOM segment, %s, had an invalid orientation."), *bottomSegment->GetName());
			return segment;
			break;
		}


		if (validSegments.Num() > 0)
		{
			// Randomly choose a valid segment, and spawn it.
			FSegmentSpawnInfo segSpawnInfo = validSegments.Array()[FMath::RandRange(0, validSegments.Num() - 1)];
			segment = Cast<ALevelSegment>(GetWorld()->SpawnActor(segSpawnInfo._segment));

			// Set the position and rotation of the segment.
			//vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);
			//horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
			segment->SetActorLocation(FVector(vertPos, horPos, 500.0f));
			setValidOrientation(segment, segSpawnInfo._validOrientations);

			return segment;
		}
		else
		{
			UE_LOG(LogLevelGen, Error, TEXT("Failed to find valid segment from both segments groups."));
			return segment;
		}
	}
}

ALevelSegment* ALevelGenerator::getValidSegment(ALevelSegment* leftSegment)
{
	ALevelSegment* segment = nullptr;
	ALevelSegment* bottomSegment = nullptr;

	ESegmentOrientations leftOrientation = ESegmentOrientations::SEGO_Right;
	ESegmentOrientations bottomOrientation = ESegmentOrientations::SEGO_Up;

	float vertPos = 0.0f;
	float horPos = 0.0f;


	if (leftSegment)
	{
		UE_LOG(LogLevelGen, Error, TEXT("Segment: %s, SHOOTING RAY"), *leftSegment->GetName());
		// Perform a line trace to the left.
		FHitResult hitResult;
		FCollisionObjectQueryParams queryParams;
		queryParams.AddObjectTypesToQuery(ECC_LevelSegmentChannel);
		FVector traceStart = leftSegment->getHOffsetLocation() + FVector(0.0f, 150.0f, 0.0f);
		FVector traceEnd = FVector(traceStart.X - 2000.0f, traceStart.Y + 100.0f, traceStart.Z);
		
		//if (GetWorld()->LineTraceSingleByObjectType(hitResult, traceStart, traceEnd, queryParams))
		//{
		//	UE_LOG(LogLevelGen, Error, TEXT("HIT SOMETHING!"));
		//	if (hitResult.bBlockingHit)
		//	{
		//		if (GEngine)
		//		{
		//			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Segment Hit: %s"), *hitResult.GetActor()->GetName()));
		//		}

		//		bottomSegment = Cast<ALevelSegment>(hitResult.GetActor());
		//		UE_LOG(LogLevelGen, Error, TEXT("Found bottom!"));
		//	}
		//	else
		//	{
		//		UE_LOG(LogLevelGen, Error, TEXT("Found bottom! NOT BLOCKING"));
		//	}
		//}

		FCollisionShape collisionShape = FCollisionShape::MakeBox(FVector(25.0f, 25.0f, 25.0f));
		FColor lineColour = FColor::Red;

		if (GetWorld()->SweepSingleByObjectType(hitResult, traceStart, traceEnd, FQuat::Identity, queryParams, collisionShape))
		{
			UE_LOG(LogLevelGen, Error, TEXT("HIT SOMETHING!"));
			if (hitResult.bBlockingHit)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Segment Hit: %s"), *hitResult.GetActor()->GetName()));
				}

				bottomSegment = Cast<ALevelSegment>(hitResult.GetActor());
				lineColour = FColor::Green;
				UE_LOG(LogLevelGen, Error, TEXT("Found bottom!"));
			}
			else
			{
				UE_LOG(LogLevelGen, Error, TEXT("Found bottom! NOT BLOCKING"));
			}
		}

		DrawDebugLine(GetWorld(), traceStart, traceEnd, lineColour, true, -1.0f, 0, 10.0f);
		DrawDebugBox(GetWorld(), traceStart, FVector(50.0f, 50.0f, 50.0f), lineColour, true);
	}


	// Most likely the beginning of the grid.
	if (!leftSegment && !bottomSegment && !_prevRowStart)
	{
		FSegmentGroup* segmentGroup = _segmentGroups.Find(ESegmentTypes::Generic);

		if (!segmentGroup)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Failed to find valid segment group from the generic segment group."));
			return segment;
		}

		// Randomly choose a valid segment, and spawn it.
		TSubclassOf<ALevelSegment> classType = segmentGroup->_validRightSegments.Array()[FMath::RandRange(0, segmentGroup->_validRightSegments.Num() - 1)];
		segment = Cast<ALevelSegment>(GetWorld()->SpawnActor(classType));
		segment->SetActorLocation(FVector(vertPos, horPos, 500.0f));

		return segment;
	}
	// Most likely the beginning of a new row.
	else if (!leftSegment && _prevRowStart)
	{
		bottomSegment = _prevRowStart;

		bottomOrientation = bottomSegment->getOrientation();

		TSet<FSegmentSpawnInfo> validSegments;

		// Get valid segments from the bottom segment.
		switch (bottomOrientation)
		{
		case ESegmentOrientations::SEGO_Right:
			validSegments = bottomSegment->getValidUpSegments();
			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has RIGHT orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Left:
			validSegments = bottomSegment->getValidDownSegments();
			vertPos = bottomSegment->GetActorLocation().X;

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has LEFT orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Up:
			validSegments = bottomSegment->getValidRightSegments();
			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has UP orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Down:
			validSegments = bottomSegment->getValidLeftSegments();
			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has DOWN orientation flag set!"), *bottomSegment->GetName());
			break;
		default:
			UE_LOG(LogLevelGen, Error, TEXT("BOTTOM segment, %s, had an invalid orientation."), *bottomSegment->GetName());
			break;
		}


		// Randomly choose a valid segment, and spawn it.
		FSegmentSpawnInfo segSpawnInfo = validSegments.Array()[FMath::RandRange(0, validSegments.Num() - 1)];
		segment = Cast<ALevelSegment>(GetWorld()->SpawnActor(segSpawnInfo._segment));

		// Set the position and rotation of the segment.
		//vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);
		segment->SetActorLocation(FVector(vertPos, horPos, 500.0f));
		setValidOrientation(segment, segSpawnInfo._validOrientations);

		return segment;
	}
	// Most likely a column on the first row.
	else if (!bottomSegment)
	{
		leftOrientation = leftSegment->getOrientation();

		TSet<FSegmentSpawnInfo> validSegments;

		// Get valid segments from the left segment.
		switch (leftOrientation)
		{
		case ESegmentOrientations::SEGO_Right:
			validSegments = leftSegment->getValidRightSegments();
			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has RIGHT orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Left:
			validSegments = leftSegment->getValidLeftSegments();
			horPos = leftSegment->GetActorLocation().Y;

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has LEFT orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Up:
			validSegments = leftSegment->getValidUpSegments();
			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has UP orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Down:
			validSegments = leftSegment->getValidDownSegments();
			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has DOWN orientation flag set!"), *leftSegment->GetName());
			break;
		default:
			UE_LOG(LogLevelGen, Error, TEXT("LEFT segment, %s, had an invalid orientation."), *leftSegment->GetName());
			break;
		}


		// Randomly choose a valid segment, and spawn it.
		FSegmentSpawnInfo segSpawnInfo = validSegments.Array()[FMath::RandRange(0, validSegments.Num() - 1)];
		segment = Cast<ALevelSegment>(GetWorld()->SpawnActor(segSpawnInfo._segment));

		// Set the position and rotation of the segment.
		//horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
		segment->SetActorLocation(FVector(vertPos, horPos, 500.0f));
		setValidOrientation(segment, segSpawnInfo._validOrientations);

		return segment;
	}
	// Both a left and bottom segment exist.
	else
	{
		leftOrientation = leftSegment->getOrientation();
		bottomOrientation = bottomSegment->getOrientation();


		// Find the segments valid for both segment types.
		//TSet<TSubclassOf<ALevelSegment>> validSegments;
		TSet<FSegmentSpawnInfo> validSegments;

		// Get valid segments from the left segment.
		switch (leftOrientation)
		{
		case ESegmentOrientations::SEGO_Right:
			validSegments = leftSegment->getValidRightSegments();
			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has RIGHT orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Left:
			validSegments = leftSegment->getValidLeftSegments();
			horPos = leftSegment->GetActorLocation().Y;

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has LEFT orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Up:
			validSegments = leftSegment->getValidUpSegments();
			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has UP orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Down:
			validSegments = leftSegment->getValidDownSegments();
			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has DOWN orientation flag set!"), *leftSegment->GetName());
			break;
		default:
			UE_LOG(LogLevelGen, Error, TEXT("LEFT segment, %s, had an invalid orientation."), *leftSegment->GetName());
			return segment;
			break;
		}

		// Get valid segments from the bottom segment.
		switch (bottomOrientation)
		{
		case ESegmentOrientations::SEGO_Right:
			validSegments = validSegments.Intersect(bottomSegment->getValidUpSegments());
			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has RIGHT orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Left:
			validSegments = validSegments.Intersect(bottomSegment->getValidDownSegments());
			vertPos = bottomSegment->GetActorLocation().X;

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has LEFT orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Up:
			validSegments = validSegments.Intersect(bottomSegment->getValidRightSegments());
			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has UP orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Down:
			validSegments = validSegments.Intersect(bottomSegment->getValidLeftSegments());
			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has DOWN orientation flag set!"), *bottomSegment->GetName());
			break;
		default:
			UE_LOG(LogLevelGen, Error, TEXT("BOTTOM segment, %s, had an invalid orientation."), *bottomSegment->GetName());
			return segment;
			break;
		}


		if (validSegments.Num() > 0)
		{
			// Randomly choose a valid segment, and spawn it.
			FSegmentSpawnInfo segSpawnInfo = validSegments.Array()[FMath::RandRange(0, validSegments.Num() - 1)];
			segment = Cast<ALevelSegment>(GetWorld()->SpawnActor(segSpawnInfo._segment));

			// Set the position and rotation of the segment.
			//vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);
			//horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
			segment->SetActorLocation(FVector(vertPos, horPos, 500.0f));
			setValidOrientation(segment, segSpawnInfo._validOrientations);

			return segment;
		}
		else
		{
			UE_LOG(LogLevelGen, Error, TEXT("Failed to find valid segment from both segments groups."));
			return segment;
		}
	}
}

void ALevelGenerator::setValidOrientation(ALevelSegment* segment, uint8 validOrientations)
{
	ESegmentOrientations orienation = ESegmentOrientations::SEGO_Right;


	// Extract all orientation flags, and randomly pick one.
	TArray<ESegmentOrientations> _allOrientations;

	if (validOrientations & static_cast<uint8>(ESegmentOrientations::SEGO_Right))
	{
		_allOrientations.Add(ESegmentOrientations::SEGO_Right);
	}
	if (validOrientations & static_cast<uint8>(ESegmentOrientations::SEGO_Left))
	{
		_allOrientations.Add(ESegmentOrientations::SEGO_Left);
	}
	if (validOrientations & static_cast<uint8>(ESegmentOrientations::SEGO_Up))
	{
		_allOrientations.Add(ESegmentOrientations::SEGO_Up);
	}
	if (validOrientations & static_cast<uint8>(ESegmentOrientations::SEGO_Down))
	{
		_allOrientations.Add(ESegmentOrientations::SEGO_Down);
	}

	if (_allOrientations.Num() > 0)
	{
		orienation = _allOrientations[FMath::RandRange(0, _allOrientations.Num() - 1)];
	}
	else
	{
		UE_LOG(LogLevelGen, Warning, TEXT("New segment, %s, no valid orientations found!"), *FText::FromName(segment->GetFName()).ToString());
	}


	// Based on orientation chosen set the orientation, rotation, and adjust the position of the new segment.
	switch (orienation)
	{
	case ESegmentOrientations::SEGO_Right:
		UE_LOG(LogLevelGen, Warning, TEXT("New segment, %s, has RIGHT orientation flag set!"), *FText::FromName(segment->GetFName()).ToString());
		break;
	case ESegmentOrientations::SEGO_Left:
	{
		segment->SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));

		FVector pos = segment->GetActorLocation();
		pos.X += (segment->getMesh()->Bounds.BoxExtent.X * 2.0f);
		pos.Y += (segment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
		segment->SetActorLocation(pos);

		UE_LOG(LogLevelGen, Warning, TEXT("New segment, %s, has LEFT orientation flag set!"), *FText::FromName(segment->GetFName()).ToString());
	}
		break;
	case ESegmentOrientations::SEGO_Up:
	{
		segment->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));

		FVector pos = segment->GetActorLocation();
		pos.Y += (segment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
		segment->SetActorLocation(pos);

		UE_LOG(LogLevelGen, Warning, TEXT("New segment, %s, has UP orientation flag set!"), *FText::FromName(segment->GetFName()).ToString());
	}
		break;
	case ESegmentOrientations::SEGO_Down:
	{
		segment->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));

		FVector pos = segment->GetActorLocation();
		pos.X += (segment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
		segment->SetActorLocation(pos);

		UE_LOG(LogLevelGen, Warning, TEXT("New segment, %s, has DOWN orientation flag set!"), *FText::FromName(segment->GetFName()).ToString());
	}
		break;
	default:
		UE_LOG(LogLevelGen, Error, TEXT("Segment, %s, has no orientation flags set!"), *FText::FromName(segment->GetFName()).ToString());
		break;
	}

	// Based on orientation chosen set the orientation, rotation, and adjust the position of the new segment.
	//if (validOrientations & static_cast<uint8>(ESegmentOrientations::SEGO_Right))
	//{
	//	UE_LOG(LogLevelGen, Warning, TEXT("New segment, %s, has RIGHT orientation flag set!"), *FText::FromName(segment->GetFName()).ToString());
	//}
	//else if (validOrientations & static_cast<uint8>(ESegmentOrientations::SEGO_Left))
	//{
	//	orienation = ESegmentOrientations::SEGO_Left;
	//	segment->SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));

	//	FVector pos = segment->GetActorLocation();
	//	pos.X += (segment->getMesh()->Bounds.BoxExtent.X * 2.0f);
	//	pos.Y += (segment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
	//	segment->SetActorLocation(pos);

	//	UE_LOG(LogLevelGen, Warning, TEXT("New segment, %s, has LEFT orientation flag set!"), *FText::FromName(segment->GetFName()).ToString());
	//}
	//else if (validOrientations & static_cast<uint8>(ESegmentOrientations::SEGO_Up))
	//{
	//	orienation = ESegmentOrientations::SEGO_Up;
	//	segment->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));

	//	FVector pos = segment->GetActorLocation();
	//	pos.Y += (segment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
	//	segment->SetActorLocation(pos);

	//	UE_LOG(LogLevelGen, Warning, TEXT("New segment, %s, has UP orientation flag set!"), *FText::FromName(segment->GetFName()).ToString());
	//}
	//else if (validOrientations & static_cast<uint8>(ESegmentOrientations::SEGO_Down))
	//{
	//	orienation = ESegmentOrientations::SEGO_Down;
	//	segment->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));

	//	FVector pos = segment->GetActorLocation();
	//	pos.X += (segment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
	//	segment->SetActorLocation(pos);

	//	UE_LOG(LogLevelGen, Warning, TEXT("New segment, %s, has DOWN orientation flag set!"), *FText::FromName(segment->GetFName()).ToString());
	//}
	//else
	//{
	//	UE_LOG(LogLevelGen, Error, TEXT("Segment, %s, has no orientation flags set!"), *FText::FromName(segment->GetFName()).ToString());
	//}

	segment->setOrientation(orienation);
}

