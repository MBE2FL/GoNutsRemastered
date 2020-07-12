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
	bool storeFirstNode = true;

	// Generate segments for each lane.
	for (int32 laneIndex = 0; laneIndex < lanes.Num(); ++laneIndex)
	{
		currLane = &lanes[laneIndex];

		// Keep generating segments until the current lane is full.
		while (!currLane->isFull())
		{
			// Spawn a valid segment.
			UE_LOG(LogLevelGen, Warning, TEXT(" "));
			UE_LOG(LogLevelGen, Warning, TEXT("Lane Index: %i"), laneIndex);
			_currSegment = getValidSegment(prevHSegment, currLane);

			// Check for segment spawn failure.
			if (!_currSegment)
			{
				UE_LOG(LogLevelGen, Error, TEXT("Segment failed to spawn!"));
				//continue;
				break;
			}

			UE_LOG(LogLevelGen, Warning, TEXT("Segment %s spawned."), *_currSegment->GetName());

			// Add new segments width to the lanes total used width.
			currLane->_usedWidth += _currSegment->getMesh()->Bounds.BoxExtent.X;


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


			// Store beginning of each row.
			if (storeFirstNode)
			{
				_prevRowStart = _currSegment;
				storeFirstNode = false;
			}

			// Store previous horizontal segment.
			prevHSegment = _currSegment;
		}


		// Reset lane for next row.
		currLane->_usedWidth = 0.0f;
	}

	// Reset for next row.
	prevHSegment = nullptr;
	storeFirstNode = true;
}

ALevelSegment* ULevelGenUpState::getValidSegment(ALevelSegment* leftSegment, FLane* lane)
{
	ALevelSegment* segment = nullptr;
	ALevelSegment* bottomSegment = nullptr;

	ESegmentOrientations leftOrientation = ESegmentOrientations::SEGO_Right;
	ESegmentOrientations bottomOrientation = ESegmentOrientations::SEGO_Up;

	float vertPos = 0.0f;
	float horPos = 0.0f;


	if (leftSegment)
	{
		//UE_LOG(LogLevelGen, Error, TEXT("Segment: %s, SHOOTING RAY"), *leftSegment->GetName());
		// Perform a line trace to the left.
		FHitResult hitResult;
		FCollisionObjectQueryParams queryParams;
		queryParams.AddObjectTypesToQuery(ECC_LevelSegmentChannel);
		FVector traceStart = leftSegment->getHOffsetLocation() + FVector(0.0f, 150.0f, 0.0f);
		FVector traceEnd = FVector(traceStart.X - 500.0f - leftSegment->getLocalVOffset(), traceStart.Y + 100.0f, traceStart.Z);

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

		if (_levelGen->GetWorld()->SweepSingleByObjectType(hitResult, traceStart, traceEnd, FQuat::Identity, queryParams, collisionShape))
		{
			//UE_LOG(LogLevelGen, Error, TEXT("HIT SOMETHING!"));
			if (hitResult.bBlockingHit)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Segment Hit: %s"), *hitResult.GetActor()->GetName()));
				}

				bottomSegment = Cast<ALevelSegment>(hitResult.GetActor());
				lineColour = FColor::Green;
				//UE_LOG(LogLevelGen, Error, TEXT("Found bottom!"));
			}
			else
			{
				UE_LOG(LogLevelGen, Error, TEXT("Found bottom! NOT BLOCKING"));
			}
		}

		DrawDebugLine(_levelGen->GetWorld(), traceStart, traceEnd, lineColour, true, -1.0f, 0, 10.0f);
		DrawDebugBox(_levelGen->GetWorld(), traceStart, FVector(50.0f, 50.0f, 50.0f), lineColour, true);
	}


	// Most likely the beginning of the grid.
	if (!leftSegment && !bottomSegment && !_prevRowStart)
	{
		// Randomly choose a valid segment, and spawn it.
		int32 numSegments = lane->_validSegments.Num();

		if (numSegments <= 0)
		{
			UE_LOG(LogLevelGen, Error, TEXT("Lane did not have any segments assigned to it!"));
			return segment;
		}

		//TSubclassOf<ALevelSegment> classType = lane->_validSegments[FMath::RandRange(0, numSegments - 1)];
		TSubclassOf<ALevelSegment> classType = lane->_validSegments.Array()[FMath::RandRange(0, numSegments - 1)];
		segment = Cast<ALevelSegment>(_levelGen->GetWorld()->SpawnActor(classType));
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
		//FSegmentSpawnInfo segSpawnInfo = validSegments.Array()[FMath::RandRange(0, validSegments.Num() - 1)];
		FSegmentSpawnInfo segSpawnInfo;

		if (!lane->getValidLaneSegment(validSegments, segSpawnInfo))
		{
			UE_LOG(LogLevelGen, Error, TEXT("Could not find a valid segment between the lane and the bottom segment: %s!"), *bottomSegment->GetName());
			return segment;
		}

		segment = Cast<ALevelSegment>(_levelGen->GetWorld()->SpawnActor(segSpawnInfo._segment));

		// Set the position and rotation of the segment.
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
		FSegmentSpawnInfo segSpawnInfo;

		if (!lane->getValidLaneSegment(validSegments, segSpawnInfo))
		{
			UE_LOG(LogLevelGen, Error, TEXT("Could not find a valid segment between the lane and the left segment: %s!"), *leftSegment->GetName());
			return segment;
		}

		segment = Cast<ALevelSegment>(_levelGen->GetWorld()->SpawnActor(segSpawnInfo._segment));

		// Set the position and rotation of the segment.
		vertPos = leftSegment->getWorldVOffset();
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
			FSegmentSpawnInfo segSpawnInfo;

			if (!lane->getValidLaneSegment(validSegments, segSpawnInfo))
			{
				UE_LOG(LogLevelGen, Error, TEXT("Could not find a valid segment between the lane and the left and bottom segments: %s, !"), *leftSegment->GetName(), *bottomSegment->GetName());
				return segment;
			}

			segment = Cast<ALevelSegment>(_levelGen->GetWorld()->SpawnActor(segSpawnInfo._segment));

			// Set the position and rotation of the segment.
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
