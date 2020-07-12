// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenLeftState.h"
#include "LevelGenUpState.h"

void ULevelGenLeftState::update()
{
	//for (int32 column = 0; column < _width; ++column)
	//{
	//	// Spawn a valid segment.
	//	UE_LOG(LogLevelGen, Warning, TEXT(" "));
	//	UE_LOG(LogLevelGen, Warning, TEXT("Column_%i"), column);
	//	_currSegment = getValidSegment(prevHSegment);

	//	// Check for segment spawn failure.
	//	if (!_currSegment)
	//	{
	//		UE_LOG(LogLevelGen, Error, TEXT("Segment failed to spawn!"));
	//		continue;
	//	}

	//	UE_LOG(LogLevelGen, Warning, TEXT("Segment %s spawned."), *FText::FromName(_currSegment->GetFName()).ToString());


	//	// Notify listeners for special segments.
	//	switch (_currSegment->getSegmentType())
	//	{
	//	case ESegmentTypes::Generic:
	//		break;
	//	case ESegmentTypes::Crosswalk:
	//		if (_currSegment->getSegmentFeatures() & ESegmentFeatures::SF_Spawn_Pedestrians)
	//		{
	//			//_onCrosswalkSpawnedEvent.Broadcast(_currSegment, _currSegment->getPedestrianSpawnPoints());
	//			_levelGen->onCrosswalkSpawned().Broadcast(_currSegment, _currSegment->getPedestrianSpawnPoints());
	//		}
	//		break;
	//	case ESegmentTypes::Road:
	//		if (_currSegment->getSegmentFeatures() & ESegmentFeatures::SF_Spawn_Obstacles)
	//		{
	//			//_onRoadSpawnedEvent.Broadcast(_currSegment, _currSegment->getObstacleSpawnPoints());
	//			_levelGen->onRoadSpawned().Broadcast(_currSegment, _currSegment->getObstacleSpawnPoints());
	//		}
	//		break;
	//	default:
	//		break;
	//	}


	//	// Store beginning of each row.
	//	if (column == 0)
	//	{
	//		_prevRowStart = _currSegment;
	//	}

	//	// Store previous horizontal segment.
	//	prevHSegment = _currSegment;
	//}

	//// Reset for next row.
	//prevHSegment = nullptr;
}

ULevelGenState* ULevelGenLeftState::updateState()
{
	// Transition to forward generation state.
	if (_levelGen->getMapOrientation() == EMapOrientations::MO_Up)
	{
		return ALevelGenerator::getLevelGenUpState();
	}
	// Stay in current generation state.
	else
	{
		return nullptr;
	}
}

ALevelSegment* ULevelGenLeftState::getValidSegment(ALevelSegment* leftSegment, FLane* lane)
{
    return nullptr;
}

void ULevelGenLeftState::setValidOrientation(ALevelSegment* segment, uint8 validOrientations)
{
}
