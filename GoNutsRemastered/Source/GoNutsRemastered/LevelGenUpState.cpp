// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenUpState.h"

#include "DrawDebugHelpers.h"
#include "LevelGenLeftState.h"

ULevelGenState* ULevelGenUpState::updateState()
{
	// Transition to right generation state.
	if (_levelGen->getMapOrientation() == EMapOrientations::MO_Right)
	{
		return nullptr;
	}
	// Transition to left generation state.
	else if (_levelGen->getMapOrientation() == EMapOrientations::MO_Left)
	{
		return _levelGen->getLevelGenLeftState();
		//return ALevelGenerator::getLevelGenLeftState();
	}
	// Stay in current generation state.
	else
	{
		return nullptr;
	}
}

void ULevelGenUpState::update()
{
	TArray<FLane> lanes = _levelGen->getLanes();
	FLane* currLane = nullptr;
	uint8 slotIndex = 0;

	// Generate segments for each lane.
	for (int32 laneIndex = 0; laneIndex < lanes.Num(); ++laneIndex)
	{
		currLane = &lanes[laneIndex];

		// Keep generating segments until the current lane is full horizontally.
		while (!currLane->isFull())
		{
			// Spawn a valid segment.
			UE_LOG(LogLevelGen, Warning, TEXT(" "));
			UE_LOG(LogLevelGen, Warning, TEXT("#################################################"));
			UE_LOG(LogLevelGen, Warning, TEXT("Lane Index: %i, Slot Index: %u"), laneIndex, slotIndex);
			_currSegment = getValidSegment(currLane, slotIndex, 2);


			// Check for segment spawn failure.
			if (!_currSegment)
			{
				UE_LOG(LogLevelGen, Error, TEXT("Segment failed to spawn!"));
				//continue;
				++slotIndex;
				break;
			}

			UE_LOG(LogLevelGen, Warning, TEXT("Segment %s spawned."), *_currSegment->GetName());

			for (uint8 SI = 0; SI < _currSegment->getHorizontalSlots(); ++SI)
			{
				if (_prevRow.IsValidIndex(slotIndex))
				{
					_prevRow[slotIndex] = _currSegment;
					UE_LOG(LogLevelGen, Error, TEXT("INDEX EXISTED! %u"), slotIndex);
				}
				else
				{
					_prevRow.Add(_currSegment);
					UE_LOG(LogLevelGen, Error, TEXT("INDEX ADDED! %u"), slotIndex);
				}

				++slotIndex;
			}
			//++slotIndex;


			// Add new segments width to the lanes total used width.
			currLane->_usedSlots += _currSegment->getHorizontalSlots();


			// Notify listeners of special segments.
			switch (_currSegment->getSegmentType())
			{
			case ESegmentTypes::Generic:
				break;
			case ESegmentTypes::Crosswalk:
				if (_currSegment->getSegmentFeatures() & ESegmentFeatures::SF_Spawn_Pedestrians)
				{
					_levelGen->onCrosswalkSpawned().Broadcast(_currSegment, _currSegment->getPedestrianSpawnPoints());
				}
				break;
			case ESegmentTypes::Road:
				if (_currSegment->getSegmentFeatures() & ESegmentFeatures::SF_Spawn_Obstacles)
				{
					_levelGen->onRoadSpawned().Broadcast(_currSegment, _currSegment->getObstacleSpawnPoints());
				}
				break;
			default:
				break;
			}
		}


		// Reset lane for next row.
		currLane->_usedSlots = 0;
	}
}

//ALevelSegment* ULevelGenUpState::getValidSegment(FLane* lane, uint8 slotIndex)
//{
//	ALevelSegment* segment = nullptr;
//	ALevelSegment* leftSegment = nullptr;
//	ALevelSegment* bottomSegment = nullptr;
//
//	ESegmentOrientations leftOrientation = ESegmentOrientations::SEGO_Right;
//	ESegmentOrientations bottomOrientation = ESegmentOrientations::SEGO_Up;
//
//	float vertPos = 0.0f;
//	float horPos = 0.0f;
//
//	findAdjacentSegments(&leftSegment, &bottomSegment, slotIndex);
//
//
//	// Most likely the beginning of the grid.
//	if (!leftSegment && !bottomSegment)
//	{
//		// Get the number of valid segment types.
//		int32 numSegmentTypes = lane->_validSegmentTypes.Num();
//
//		if (numSegmentTypes <= 0)
//		{
//			UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any segment types assigned to it!"));
//			return segment;
//		}
//
//		// Get a random valid segment type.
//		ESegmentTypes segmentType = lane->_validSegmentTypes.Array()[FMath::RandRange(0, numSegmentTypes - 1)];
//		FSegmentClassTypes* classTypesInfo = _levelGen->getSegments().Find(segmentType);
//
//		if (!classTypesInfo)
//		{
//			UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any class types assigned to a segment type!"));
//			return segment;
//		}
//
//		TArray<TSubclassOf<ALevelSegment>> classTypes = classTypesInfo->_segmentClassTypes;
//		if (classTypes.Num() <= 0)
//		{
//			UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any class types assigned to a segment type!"));
//			return segment;
//		}
//
//		// Spawn a segment of the chosen type.
//		TSubclassOf<ALevelSegment> classType = classTypes[FMath::RandRange(0, classTypes.Num() - 1)];
//		segment = Cast<ALevelSegment>(_levelGen->GetWorld()->SpawnActor(classType));
//		segment->SetActorLocation(FVector(vertPos, horPos, 500.0f));
//
//		return segment;
//	}
//	// Most likely the beginning of a new row.
//	else if (!leftSegment)
//	{
//		bottomOrientation = bottomSegment->getOrientation();
//
//		TSet<FSegmentSpawnInfo> validSegmentTypes;
//
//		// Get valid segment types from the bottom segment.
//		switch (bottomOrientation)
//		{
//		case ESegmentOrientations::SEGO_Right:
//			//validSegmentTypes = bottomSegment->getValidUpSegments();
//			validSegmentTypes = _levelGen->getValidTopSegments(bottomSegment->getSegmentType());
//			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);
//
//			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has RIGHT orientation flag set!"), *bottomSegment->GetName());
//			break;
//		case ESegmentOrientations::SEGO_Left:
//			//validSegmentTypes = bottomSegment->getValidDownSegments();
//			validSegmentTypes = _levelGen->getValidBottomSegments(bottomSegment->getSegmentType());
//			vertPos = bottomSegment->GetActorLocation().X;
//
//			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has LEFT orientation flag set!"), *bottomSegment->GetName());
//			break;
//		case ESegmentOrientations::SEGO_Up:
//			//validSegmentTypes = bottomSegment->getValidRightSegments();
//			validSegmentTypes = _levelGen->getValidRightSegments(bottomSegment->getSegmentType());
//			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);
//
//			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has UP orientation flag set!"), *bottomSegment->GetName());
//			break;
//		case ESegmentOrientations::SEGO_Down:
//			//validSegmentTypes = bottomSegment->getValidLeftSegments();
//			validSegmentTypes = _levelGen->getValidLeftSegments(bottomSegment->getSegmentType());
//			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);
//
//			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has DOWN orientation flag set!"), *bottomSegment->GetName());
//			break;
//		default:
//			UE_LOG(LogLevelGen, Error, TEXT("BOTTOM segment, %s, had an invalid orientation."), *bottomSegment->GetName());
//			break;
//		}
//
//
//		// Get a random valid segment type.
//		//FSegmentSpawnInfo segSpawnInfo = validSegments.Array()[FMath::RandRange(0, validSegments.Num() - 1)];
//		FSegmentSpawnInfo segSpawnInfo;
//
//		// Check for valid segment types between the lane and the bottom segment.
//		if (!lane->getValidLaneSegment(validSegmentTypes, segSpawnInfo))
//		{
//			UE_LOG(LogLevelGen, Error, TEXT("Could not find a valid segment type between the lane and the bottom segment: %s!"), *bottomSegment->GetName());
//			return segment;
//		}
//
//		// Get all the segments for the specified segment type.
//		FSegmentClassTypes* classTypesInfo = _levelGen->getSegments().Find(segSpawnInfo._segmentType);
//		TArray<TSubclassOf<ALevelSegment>> classTypes = classTypesInfo->_segmentClassTypes;
//		if (classTypes.Num() <= 0)
//		{
//			UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any class types assigned to a segment type!"));
//			return segment;
//		}
//
//		// Spawn a random segment of the chosen type.
//		TSubclassOf<ALevelSegment> classType = classTypes[FMath::RandRange(0, classTypes.Num() - 1)];
//		segment = Cast<ALevelSegment>(_levelGen->GetWorld()->SpawnActor(classType));
//
//		// Set the position and rotation of the segment.
//		segment->SetActorLocation(FVector(vertPos, horPos, 500.0f));
//		setValidOrientation(segment, segSpawnInfo._validOrientations);
//
//		return segment;
//	}
//	// Most likely a column on the first row.
//	else if (!bottomSegment)
//	{
//		leftOrientation = leftSegment->getOrientation();
//
//		TSet<FSegmentSpawnInfo> validSegments;
//
//		// Get valid segments from the left segment.
//		switch (leftOrientation)
//		{
//		case ESegmentOrientations::SEGO_Right:
//			validSegments = _levelGen->getValidRightSegments(leftSegment->getSegmentType());
//			horPos = leftSegment->getHOffset();
//
//			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has RIGHT orientation flag set!"), *leftSegment->GetName());
//			break;
//		case ESegmentOrientations::SEGO_Left:
//			validSegments = _levelGen->getValidLeftSegments(leftSegment->getSegmentType());
//			horPos = leftSegment->getHOffset();
//
//			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has LEFT orientation flag set!"), *leftSegment->GetName());
//			break;
//		case ESegmentOrientations::SEGO_Up:
//			validSegments = _levelGen->getValidTopSegments(leftSegment->getSegmentType());
//			horPos = leftSegment->getHOffset();
//
//			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has UP orientation flag set!"), *leftSegment->GetName());
//			break;
//		case ESegmentOrientations::SEGO_Down:
//			validSegments = _levelGen->getValidBottomSegments(leftSegment->getSegmentType());
//			horPos = leftSegment->getHOffset();
//
//			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has DOWN orientation flag set!"), *leftSegment->GetName());
//			break;
//		default:
//			UE_LOG(LogLevelGen, Error, TEXT("LEFT segment, %s, had an invalid orientation."), *leftSegment->GetName());
//			break;
//		}
//
//
//		// Get a random valid segment type.
//		FSegmentSpawnInfo segSpawnInfo;
//
//		// Check for valid segment types between the lane and the left segment.
//		if (!lane->getValidLaneSegment(validSegments, segSpawnInfo))
//		{
//			UE_LOG(LogLevelGen, Error, TEXT("Could not find a valid segment type between the lane and the left segment: %s!"), *leftSegment->GetName());
//			return segment;
//		}
//
//		// Get all the segments for the specified segment type.
//		FSegmentClassTypes* classTypesInfo = _levelGen->getSegments().Find(segSpawnInfo._segmentType);
//		TArray<TSubclassOf<ALevelSegment>> classTypes = classTypesInfo->_segmentClassTypes;
//
//		if (classTypes.Num() <= 0)
//		{
//			UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any class types assigned to a segment type!"));
//			return segment;
//		}
//
//
//		// Spawn a segment of the chosen type.
//		TSubclassOf<ALevelSegment> classType = classTypes[FMath::RandRange(0, classTypes.Num() - 1)];
//		segment = Cast<ALevelSegment>(_levelGen->GetWorld()->SpawnActor(classType));
//
//		// Set the position and rotation of the segment.
//		vertPos = leftSegment->getLocalVerticalValue();
//		segment->SetActorLocation(FVector(vertPos, horPos, 500.0f));
//		setValidOrientation(segment, segSpawnInfo._validOrientations);
//
//		return segment;
//	}
//	// Both a left and bottom segment exist.
//	else
//	{
//		leftOrientation = leftSegment->getOrientation();
//		bottomOrientation = bottomSegment->getOrientation();
//
//
//		// Find the segments valid for both segment types.
//		//TSet<TSubclassOf<ALevelSegment>> validSegments;
//		TSet<FSegmentSpawnInfo> validSegments;
//
//		// Get valid segments from the left segment.
//		switch (leftOrientation)
//		{
//		case ESegmentOrientations::SEGO_Right:
//			validSegments = _levelGen->getValidRightSegments(leftSegment->getSegmentType());
//			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
//
//			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has RIGHT orientation flag set!"), *leftSegment->GetName());
//			break;
//		case ESegmentOrientations::SEGO_Left:
//			validSegments = _levelGen->getValidLeftSegments(leftSegment->getSegmentType());
//			horPos = leftSegment->GetActorLocation().Y;
//
//			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has LEFT orientation flag set!"), *leftSegment->GetName());
//			break;
//		case ESegmentOrientations::SEGO_Up:
//			validSegments = _levelGen->getValidTopSegments(leftSegment->getSegmentType());
//			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
//
//			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has UP orientation flag set!"), *leftSegment->GetName());
//			break;
//		case ESegmentOrientations::SEGO_Down:
//			validSegments = _levelGen->getValidBottomSegments(leftSegment->getSegmentType());
//			horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);
//
//			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has DOWN orientation flag set!"), *leftSegment->GetName());
//			break;
//		default:
//			UE_LOG(LogLevelGen, Error, TEXT("LEFT segment, %s, had an invalid orientation."), *leftSegment->GetName());
//			return segment;
//			break;
//		}
//
//		// Get valid segments from the bottom segment.
//		switch (bottomOrientation)
//		{
//		case ESegmentOrientations::SEGO_Right:
//			validSegments = validSegments.Intersect(_levelGen->getValidTopSegments(bottomSegment->getSegmentType()));
//			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);
//
//			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has RIGHT orientation flag set!"), *bottomSegment->GetName());
//			break;
//		case ESegmentOrientations::SEGO_Left:
//			validSegments = validSegments.Intersect(_levelGen->getValidBottomSegments(bottomSegment->getSegmentType()));
//			vertPos = bottomSegment->GetActorLocation().X;
//
//			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has LEFT orientation flag set!"), *bottomSegment->GetName());
//			break;
//		case ESegmentOrientations::SEGO_Up:
//			validSegments = validSegments.Intersect(_levelGen->getValidRightSegments(bottomSegment->getSegmentType()));
//			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);
//
//			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has UP orientation flag set!"), *bottomSegment->GetName());
//			break;
//		case ESegmentOrientations::SEGO_Down:
//			validSegments = validSegments.Intersect(_levelGen->getValidLeftSegments(bottomSegment->getSegmentType()));
//			vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);
//
//			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has DOWN orientation flag set!"), *bottomSegment->GetName());
//			break;
//		default:
//			UE_LOG(LogLevelGen, Error, TEXT("BOTTOM segment, %s, had an invalid orientation."), *bottomSegment->GetName());
//			return segment;
//			break;
//		}
//
//
//		if (validSegments.Num() > 0)
//		{
//			// Get a random valid segment type.
//			FSegmentSpawnInfo segSpawnInfo;
//
//			// Check for valid segment types between the lane and the left and bottom segments.
//			if (!lane->getValidLaneSegment(validSegments, segSpawnInfo))
//			{
//				UE_LOG(LogLevelGen, Error, TEXT("Could not find a valid segment type between the lane and the left and bottom segments: %s, !"), *leftSegment->GetName(), *bottomSegment->GetName());
//				return segment;
//			}
//
//			// Get all the segments for the specified segment type.
//			FSegmentClassTypes* classTypesInfo = _levelGen->getSegments().Find(segSpawnInfo._segmentType);
//			TArray<TSubclassOf<ALevelSegment>> classTypes = classTypesInfo->_segmentClassTypes;
//
//			if (classTypes.Num() <= 0)
//			{
//				UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any class types assigned to a segment type!"));
//				return segment;
//			}
//
//			// Spawn a segment of the chosen type.
//			TSubclassOf<ALevelSegment> classType = classTypes[FMath::RandRange(0, classTypes.Num() - 1)];
//			segment = Cast<ALevelSegment>(_levelGen->GetWorld()->SpawnActor(classType));
//
//			// Set the position and rotation of the segment.
//			segment->SetActorLocation(FVector(vertPos, horPos, 500.0f));
//			setValidOrientation(segment, segSpawnInfo._validOrientations);
//
//			return segment;
//		}
//		else
//		{
//			UE_LOG(LogLevelGen, Error, TEXT("Failed to find valid segment from both segments groups."));
//			return segment;
//		}
//	}
//}

ALevelSegment* ULevelGenUpState::getValidSegment(FLane* lane, uint8 slotIndex, int32 ver)
{
	ALevelSegment* segment = nullptr;
	ALevelSegment* leftSegment = nullptr;
	ALevelSegment* bottomSegment = nullptr;

	ESegmentOrientations leftOrientation = ESegmentOrientations::SEGO_Right;
	ESegmentOrientations bottomOrientation = ESegmentOrientations::SEGO_Up;


	findAdjacentSegments(&leftSegment, &bottomSegment, slotIndex);


	// Most likely the beginning of the grid.
	if (!leftSegment && !bottomSegment)
	{
		// Get the number of valid segment types.
		int32 numSegmentTypes = lane->_validSegmentTypes.Num();

		if (numSegmentTypes <= 0)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any segment types assigned to it!"));
			return segment;
		}

		// Get a random valid segment type.
		ESegmentTypes segmentType = lane->_validSegmentTypes.Array()[FMath::RandRange(0, numSegmentTypes - 1)];
		FSegmentClassTypes* classTypesInfo = _levelGen->getSegments().Find(segmentType);

		if (!classTypesInfo)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any class types assigned to a segment type!"));
			return segment;
		}

		TArray<TSubclassOf<ALevelSegment>> classTypes = classTypesInfo->_segmentClassTypes;
		if (classTypes.Num() <= 0)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any class types assigned to a segment type!"));
			return segment;
		}

		// Spawn a segment of the chosen type.
		TSubclassOf<ALevelSegment> classType = classTypes[FMath::RandRange(0, classTypes.Num() - 1)];
		segment = Cast<ALevelSegment>(_levelGen->GetWorld()->SpawnActor(classType));
		segment->SetActorLocation(FVector(0.0f, 0.0f, 500.0f));

		return segment;
	}
	// Most likely the beginning of a new row.
	else if (!leftSegment)
	{
		bottomOrientation = bottomSegment->getOrientation();

		TSet<FSegmentSpawnInfo> validSegmentTypes;

		// Get valid segment types from the bottom segment.
		switch (bottomOrientation)
		{
		case ESegmentOrientations::SEGO_Right:
			validSegmentTypes = _levelGen->getValidTopSegments(bottomSegment->getSegmentType());
			//vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has RIGHT orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Left:
			validSegmentTypes = _levelGen->getValidBottomSegments(bottomSegment->getSegmentType());
			//vertPos = bottomSegment->GetActorLocation().X;

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has LEFT orientation flag set!"), *bottomSegment->GetName());
			break;
		default:
			UE_LOG(LogLevelGen, Error, TEXT("BOTTOM segment, %s, had an invalid orientation."), *bottomSegment->GetName());
			break;
		}


		// Get a random valid segment type.
		FSegmentSpawnInfo segSpawnInfo;

		// Check for valid segment types between the lane and the bottom segment.
		if (!lane->getValidLaneSegment(validSegmentTypes, segSpawnInfo))
		{
			UE_LOG(LogLevelGen, Error, TEXT("Could not find a valid segment type between the lane and the bottom segment: %s!"), *bottomSegment->GetName());
			return segment;
		}

		// Get all the segments for the specified segment type.
		FSegmentClassTypes* classTypesInfo = _levelGen->getSegments().Find(segSpawnInfo._segmentType);
		TArray<TSubclassOf<ALevelSegment>> classTypes = classTypesInfo->_segmentClassTypes;
		if (classTypes.Num() <= 0)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any class types assigned to a segment type!"));
			return segment;
		}

		// Spawn a random segment of the chosen type.
		TSubclassOf<ALevelSegment> classType = classTypes[FMath::RandRange(0, classTypes.Num() - 1)];
		segment = Cast<ALevelSegment>(_levelGen->GetWorld()->SpawnActor(classType));

		// Set the position and rotation of the segment.
		FVector spawnPoint = bottomSegment->getNextSegmentSpawnPoint();
		spawnPoint.X += bottomSegment->getVerticalSlots() * SLOT_SIZE;
		spawnPoint.Y = 0.0f;
		spawnPoint.Z = 500.0f;
		segment->SetActorLocation(spawnPoint);
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
			validSegments = _levelGen->getValidRightSegments(leftSegment->getSegmentType());
			//horPos = leftSegment->getHOffset();

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has RIGHT orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Left:
			validSegments = _levelGen->getValidLeftSegments(leftSegment->getSegmentType());
			//horPos = leftSegment->getHOffset();

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has LEFT orientation flag set!"), *leftSegment->GetName());
			break;
		default:
			UE_LOG(LogLevelGen, Error, TEXT("LEFT segment, %s, had an invalid orientation."), *leftSegment->GetName());
			break;
		}


		// Get a random valid segment type.
		FSegmentSpawnInfo segSpawnInfo;

		// Check for valid segment types between the lane and the left segment.
		if (!lane->getValidLaneSegment(validSegments, segSpawnInfo))
		{
			UE_LOG(LogLevelGen, Error, TEXT("Could not find a valid segment type between the lane and the left segment: %s!"), *leftSegment->GetName());
			return segment;
		}

		// Get all the segments for the specified segment type.
		FSegmentClassTypes* classTypesInfo = _levelGen->getSegments().Find(segSpawnInfo._segmentType);
		TArray<TSubclassOf<ALevelSegment>> classTypes = classTypesInfo->_segmentClassTypes;

		if (classTypes.Num() <= 0)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any class types assigned to a segment type!"));
			return segment;
		}


		// Spawn a segment of the chosen type.
		TSubclassOf<ALevelSegment> classType = classTypes[FMath::RandRange(0, classTypes.Num() - 1)];
		segment = Cast<ALevelSegment>(_levelGen->GetWorld()->SpawnActor(classType));

		// Set the position and rotation of the segment.
		FVector spawnPoint = leftSegment->getNextSegmentSpawnPoint();
		spawnPoint.Z = 500.0f;
		segment->SetActorLocation(spawnPoint);
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
			validSegments = _levelGen->getValidRightSegments(leftSegment->getSegmentType());
			//horPos = leftSegment->GetActorLocation().Y + (leftSegment->getMesh()->Bounds.BoxExtent.Y * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has RIGHT orientation flag set!"), *leftSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Left:
			validSegments = _levelGen->getValidLeftSegments(leftSegment->getSegmentType());
			//horPos = leftSegment->GetActorLocation().Y;

			UE_LOG(LogLevelGen, Warning, TEXT("LEFT segment, %s, has LEFT orientation flag set!"), *leftSegment->GetName());
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
			validSegments = validSegments.Intersect(_levelGen->getValidTopSegments(bottomSegment->getSegmentType()));
			//vertPos = bottomSegment->GetActorLocation().X + (bottomSegment->getMesh()->Bounds.BoxExtent.X * 2.0f);

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has RIGHT orientation flag set!"), *bottomSegment->GetName());
			break;
		case ESegmentOrientations::SEGO_Left:
			validSegments = validSegments.Intersect(_levelGen->getValidBottomSegments(bottomSegment->getSegmentType()));
			//vertPos = bottomSegment->GetActorLocation().X;

			UE_LOG(LogLevelGen, Warning, TEXT("BOTTOM segment, %s, has LEFT orientation flag set!"), *bottomSegment->GetName());
			break;
		default:
			UE_LOG(LogLevelGen, Error, TEXT("BOTTOM segment, %s, had an invalid orientation."), *bottomSegment->GetName());
			return segment;
			break;
		}


		if (validSegments.Num() > 0)
		{
			// Get a random valid segment type.
			FSegmentSpawnInfo segSpawnInfo;

			// Check for valid segment types between the lane and the left and bottom segments.
			if (!lane->getValidLaneSegment(validSegments, segSpawnInfo))
			{
				UE_LOG(LogLevelGen, Error, TEXT("Could not find a valid segment type between the lane and the left and bottom segments: %s, !"), *leftSegment->GetName(), *bottomSegment->GetName());
				return segment;
			}

			// Get all the segments for the specified segment type.
			FSegmentClassTypes* classTypesInfo = _levelGen->getSegments().Find(segSpawnInfo._segmentType);
			TArray<TSubclassOf<ALevelSegment>> classTypes = classTypesInfo->_segmentClassTypes;

			if (classTypes.Num() <= 0)
			{
				UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any class types assigned to a segment type!"));
				return segment;
			}

			// Spawn a segment of the chosen type.
			TSubclassOf<ALevelSegment> classType = classTypes[FMath::RandRange(0, classTypes.Num() - 1)];
			segment = Cast<ALevelSegment>(_levelGen->GetWorld()->SpawnActor(classType));

			// Set the position and rotation of the segment.
			FVector spawnPoint = leftSegment->getNextSegmentSpawnPoint();
			spawnPoint.Z = 500.0f;
			segment->SetActorLocation(spawnPoint);
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

void ULevelGenUpState::findAdjacentSegments(ALevelSegment** outLeftSegment, ALevelSegment** outBottomSegment, uint8 slotIndex)
{
	//if (_prevRow.IsValidIndex(slotIndex))
	//{
	//	*outBottomSegment = _prevRow[slotIndex];


	//	if (*outBottomSegment)
	//	{
	//		// Perform a line trace to the left.
	//		FHitResult hitResult;
	//		FCollisionObjectQueryParams queryParams;
	//		queryParams.AddObjectTypesToQuery(ECC_LevelSegmentChannel);

	//		//FVector traceStart = (*outBottomSegment)->getWorldVerticalOffsetLocation() + FVector(HALF_SLOT_SIZE, slotIndex * SLOT_SIZE, 0.0f);
	//		FVector traceStart = FVector((*outBottomSegment)->getWorldMaxVerticalValue() + HALF_SLOT_SIZE, slotIndex * SLOT_SIZE, (*outBottomSegment)->GetActorLocation().Z);	// TO-DO Add in start grid Y location.
	//		FVector traceEnd = FVector(traceStart.X, traceStart.Y - (*outBottomSegment)->getLocalHOffset() - DOUBLE_SLOT_SIZE, traceStart.Z);


	//		FCollisionShape collisionShape = FCollisionShape::MakeBox(FVector(25.0f, 25.0f, 25.0f));
	//		FColor lineColour = FColor::Red;

	//		if (_levelGen->GetWorld()->SweepSingleByObjectType(hitResult, traceStart, traceEnd, FQuat::Identity, queryParams, collisionShape))
	//		{
	//			//UE_LOG(LogLevelGen, Error, TEXT("HIT SOMETHING!"));
	//			if (hitResult.bBlockingHit)
	//			{
	//				if (GEngine)
	//				{
	//					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Segment Hit: %s"), *hitResult.GetActor()->GetName()));
	//				}

	//				*outLeftSegment = Cast<ALevelSegment>(hitResult.GetActor());
	//				lineColour = FColor::Green;
	//				UE_LOG(LogLevelGen, Error, TEXT("Found left!"));
	//			}
	//			else
	//			{
	//				UE_LOG(LogLevelGen, Error, TEXT("Found Left! NOT BLOCKING"));
	//			}
	//		}

	//		DrawDebugLine(_levelGen->GetWorld(), traceStart, traceEnd, lineColour, true, -1.0f, 0, 10.0f);
	//		DrawDebugBox(_levelGen->GetWorld(), traceStart, FVector(50.0f, 50.0f, 50.0f), lineColour, true);
	//	}
	//}
	//else if (_prevRow.IsValidIndex(slotIndex - 1))
	//{
	//	
	//	*outLeftSegment = _prevRow[slotIndex - 1];
	//	//*outLeftSegment = _prevRow[0];

	//	UE_LOG(LogLevelGen, Error, TEXT("Found left TWO! SI: %u"), (slotIndex - 1));

	//	if (!(*outLeftSegment))
	//	{
	//		UE_LOG(LogLevelGen, Error, TEXT("SEGMENT NOT THERE!"));
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogLevelGen, Error, TEXT("Found NOTHING!"));
	//}




	bool leftExists = _prevRow.IsValidIndex(slotIndex - 1);
	bool bottomExists = _prevRow.IsValidIndex(slotIndex);

	if (leftExists && bottomExists)
	{
		*outLeftSegment = _prevRow[slotIndex - 1];
		*outBottomSegment = _prevRow[slotIndex];
	}
	else if (bottomExists)
	{
		*outBottomSegment = _prevRow[slotIndex];
	}
	else if (leftExists)
	{
		*outLeftSegment = _prevRow[slotIndex - 1];
	}
	else
	{
		return;
	}
}

void ULevelGenUpState::setValidOrientation(ALevelSegment* segment, uint8 validOrientations)
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


	segment->setOrientation(orienation);
}
