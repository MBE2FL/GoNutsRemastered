// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "Engine/LevelStreaming.h"
#include "EngineUtils.h"
#include "LevelSegment.h"

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
	ALevelSegment* prevVSegment = nullptr;
	TSubclassOf<ALevelSegment> classType;
	float vertPos = 0.0f;
	TArray<ALevelSegment*> prevRow = TArray<ALevelSegment*>();
	ESegmentTypes hSegmentType = ESegmentTypes::Generic;
	ESegmentTypes vSegmentType = ESegmentTypes::Generic;

	for (int32 row = 0; row < _numRows; ++row)
	{
		for (int32 column = 0; column < _numColumns; ++column)
		{
			if (prevRow.IsValidIndex(column))
			{
				vSegmentType = prevRow[column]->getSegmentType();
				prevVSegment = prevRow[column];
			}


			// Spawn a valid segment.
			//segment = getValidSegment(prevHSegment, prevVSegment);+
			UE_LOG(LogLevelGen, Warning, TEXT(" "));
			UE_LOG(LogLevelGen, Warning, TEXT("Row_%i, Column_%i"), row, column);
			segment = getValidSegment(prevHSegment, prevVSegment, 0);

			// Check for segment spawn failure.
			if (!segment)
			{
				UE_LOG(LogLevelGen, Error, TEXT("Segment failed to spawn!"));
				continue;
			}

			UE_LOG(LogLevelGen, Warning, TEXT("Segment %s spawned."), *FText::FromName(segment->GetFName()).ToString());


			float horPos = 0.0f;

			// Use a previous segment to figure out horizontal position.
			if (prevHSegment)
			{
				horPos = prevHSegment->getHOffset();

				horPos = prevHSegment->GetActorLocation().Y + (prevHSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

				//UE_LOG(LogLevelGen, Warning, TEXT("Bounds Extends: %s"), *prevHSegment->getMesh()->Bounds.BoxExtent.ToString());
			}
			
			// Use the previous vertical segment, iff a previous row exists, to figure out vertical position.
			if (prevRow.IsValidIndex(column))
			{
				prevVSegment = prevRow[column];

				if (prevVSegment)
				{
					vertPos = prevVSegment->GetActorLocation().X + (prevVSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);
				}

				// Replace previous segment.
				prevRow[column] = segment;
			}
			else
			{
				// Add a new segment.
				prevRow.Add(segment);
			}

			//FTransform transform = FTransform(FRotator::ZeroRotator, FVector(vertPos, horPos, 500.0f), segment->GetActorScale());
			//segment->SetActorTransform(transform);

			prevHSegment = segment;
			hSegmentType = segment->getSegmentType();


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


		//USceneComponent* vSlot = prevSegment->getNextVSlot();

		//if (!vSlot)
		//{
		//	UE_LOG(LogLevelGen, Error, TEXT("Level Gen Actor does not have the required vertical slot!"));
		//	continue;
		//}
		//else
		//{
		//	//vertPos = vSlot->GetComponentLocation().X;
		//}

		//if (prevHSegment)
		//{
		//	vertPos = prevHSegment->GetActorLocation().X + (prevHSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);
		//}


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

ALevelSegment* ALevelGenerator::getValidSegment(ESegmentTypes hSegmentType, ESegmentTypes vSegmentType)
{
	//// Get valid segment groups based on the current horizontal and vertical segments.
	//FSegmentGroup* hSegmentGroup = _segmentGroups.Find(hSegmentType);
	//FSegmentGroup* vSegmentGroup = _segmentGroups.Find(vSegmentType);
	//

	//ALevelSegment* segment = nullptr;

	//if (!hSegmentGroup)
	//{
	//	UE_LOG(LogLevelGen, Error, TEXT("Failed to find valid segment group from horizontal segment."));
	//	return segment;
	//}

	//if (!vSegmentGroup)
	//{
	//	UE_LOG(LogLevelGen, Error, TEXT("Failed to find valid segment group from vertical segment."));
	//	return segment;
	//}

	//// Find the segments valid for both segment types.
	//TSet<TSubclassOf<ALevelSegment>> validSegments = hSegmentGroup->_validHSegments.Intersect(vSegmentGroup->_validVSegments);

	//if (validSegments.Num() > 0)
	//{
	//	// Randomly choose a valid segment, and spawn it.
	//	TSubclassOf<ALevelSegment> classType = validSegments.Array()[FMath::RandRange(0, validSegments.Num() - 1)];
	//	segment = Cast<ALevelSegment>(GetWorld()->SpawnActor(classType));

	//	return segment;
	//}
	//else
	//{
	//	UE_LOG(LogLevelGen, Error, TEXT("Failed to find valid segment from both segments groups."));
	//	return segment;
	//}


	return nullptr;
}

ALevelSegment* ALevelGenerator::getValidSegment(ALevelSegment* leftSegment, ALevelSegment* bottomSegment)
{
	ALevelSegment* segment = nullptr;

	ESegmentTypes leftSegmentType = ESegmentTypes::Generic;
	ESegmentTypes bottomSegmentType = ESegmentTypes::Generic;

	ESegmentOrientations leftOrientation = ESegmentOrientations::SEGO_Right;
	ESegmentOrientations bottomOrientation = ESegmentOrientations::SEGO_Up;

	FSegmentGroup* leftSegmentGroup = nullptr;
	FSegmentGroup* bottomSegmentGroup = nullptr;



	if (!leftSegment && !bottomSegment)
	{
		FSegmentGroup* segmentGroup = _segmentGroups.Find(ESegmentTypes::Generic);

		if (!segmentGroup)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Failed to find valid segment group from the generic segment group."));
			return segment;
		}

		TSubclassOf<ALevelSegment> classType = segmentGroup->_validRightSegments.Array()[FMath::RandRange(0, segmentGroup->_validRightSegments.Num() - 1)];
		segment = Cast<ALevelSegment>(GetWorld()->SpawnActor(classType));

		return segment;
	}
	else if (!leftSegment)
	{
		bottomSegmentType = bottomSegment->getSegmentType();

		bottomOrientation = bottomSegment->getOrientation();

		leftSegmentGroup = _segmentGroups.Find(ESegmentTypes::Generic);
		bottomSegmentGroup = _segmentGroups.Find(bottomSegmentType);
	}
	else if (!bottomSegment)
	{
		leftSegmentType = leftSegment->getSegmentType();

		leftOrientation = leftSegment->getOrientation();

		leftSegmentGroup = _segmentGroups.Find(leftSegmentType);
		bottomSegmentGroup = _segmentGroups.Find(ESegmentTypes::Generic);
	}
	else
	{
		leftSegmentType = leftSegment->getSegmentType();
		bottomSegmentType = bottomSegment->getSegmentType();

		leftOrientation = leftSegment->getOrientation();
		bottomOrientation = bottomSegment->getOrientation();

		leftSegmentGroup = _segmentGroups.Find(leftSegmentType);
		bottomSegmentGroup = _segmentGroups.Find(bottomSegmentType);
	}




	// Get valid segment groups based on the current horizontal and vertical segments.

	if (!leftSegmentGroup)
	{
		UE_LOG(LogLevelGen, Error, TEXT("Failed to find valid segment group from the left segment."));
		return segment;
	}

	if (!bottomSegmentGroup)
	{
		UE_LOG(LogLevelGen, Error, TEXT("Failed to find valid segment group from the bottom segment."));
		return segment;
	}

	// Find the segments valid for both segment types.
	TSet<TSubclassOf<ALevelSegment>> validSegments;

	switch (leftOrientation)
	{
	case ESegmentOrientations::SEGO_Right:
		validSegments = leftSegmentGroup->_validRightSegments;
		break;
	case ESegmentOrientations::SEGO_Left:
		validSegments = leftSegmentGroup->_validLeftSegments;
		break;
	case ESegmentOrientations::SEGO_Up:
		validSegments = leftSegmentGroup->_validUpSegments;
		break;
	case ESegmentOrientations::SEGO_Down:
		validSegments = leftSegmentGroup->_validDownSegments;
		break;
	default:
		UE_LOG(LogLevelGen, Error, TEXT("Left segment had an invalid orientation."));
		return segment;
		break;
	}

	switch (bottomOrientation)
	{
	case ESegmentOrientations::SEGO_Right:
		validSegments = validSegments.Intersect(bottomSegmentGroup->_validUpSegments);
		break;
	case ESegmentOrientations::SEGO_Left:
		validSegments = validSegments.Intersect(bottomSegmentGroup->_validDownSegments);
		break;
	case ESegmentOrientations::SEGO_Up:
		validSegments = validSegments.Intersect(bottomSegmentGroup->_validRightSegments);
		break;
	case ESegmentOrientations::SEGO_Down:
		validSegments = validSegments.Intersect(bottomSegmentGroup->_validLeftSegments);
		break;
	default:
		UE_LOG(LogLevelGen, Error, TEXT("Bottom segment had an invalid orientation."));
		return segment;
		break;
	}


	if (validSegments.Num() > 0)
	{
		// Randomly choose a valid segment, and spawn it.
		TSubclassOf<ALevelSegment> classType = validSegments.Array()[FMath::RandRange(0, validSegments.Num() - 1)];
		segment = Cast<ALevelSegment>(GetWorld()->SpawnActor(classType));

		return segment;
	}
	else
	{
		UE_LOG(LogLevelGen, Error, TEXT("Failed to find valid segment from both segments groups."));
		return segment;
	}
}

ALevelSegment* ALevelGenerator::getValidSegment(ALevelSegment* leftSegment, ALevelSegment* bottomSegment, int ver)
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

void ALevelGenerator::setValidOrientation(ALevelSegment* segment, ESegmentOrientations leftOrientation, ESegmentTypes leftType, 
											ESegmentTypes bottomType, ESegmentOrientations bottomOrientation)
{
	ESegmentOrientations orientation = ESegmentOrientations::SEGO_Right;
	ESegmentTypes segmentType = segment->getSegmentType();


	switch (leftType)
	{
	case ESegmentTypes::Crosswalk:
		break;
	case ESegmentTypes::House:
		break;
	// Left Segment: Road
	case ESegmentTypes::Road:
		if (leftOrientation == ESegmentOrientations::SEGO_Right || leftOrientation == ESegmentOrientations::SEGO_Left)
		{
			if (segmentType == ESegmentTypes::House || segmentType == ESegmentTypes::Sidewalk)
			{
				segment->setOrientation(ESegmentOrientations::SEGO_Left);
			}
		}
		else if (leftOrientation == ESegmentOrientations::SEGO_Up || leftOrientation == ESegmentOrientations::SEGO_Down)
		{
			if (segmentType == ESegmentTypes::House || segmentType == ESegmentTypes::Sidewalk)
			{
				segment->setOrientation(ESegmentOrientations::SEGO_Down);
			}
		}
		break;
	case ESegmentTypes::Sidewalk:
		break;
	}



	switch (leftOrientation)
	{
	case ESegmentOrientations::SEGO_Right:
		break;
	case ESegmentOrientations::SEGO_Left:
		break;
	case ESegmentOrientations::SEGO_Up:
		break;
	case ESegmentOrientations::SEGO_Down:
		break;
	default:
		break;
	}

	switch (bottomOrientation)
	{
	case ESegmentOrientations::SEGO_Right:
		break;
	case ESegmentOrientations::SEGO_Left:
		break;
	case ESegmentOrientations::SEGO_Up:
		break;
	case ESegmentOrientations::SEGO_Down:
		break;
	default:
		break;
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

