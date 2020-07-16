// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
//#include "Engine/LevelStreaming.h"
//#include "EngineUtils.h"
//#include "LevelSegment.h"
#include "LevelGenUpState.h"
#include "LevelGenLeftState.h"

#include "AssetRegistryModule.h"

DEFINE_LOG_CATEGORY(LogLevelGen);


ULevelGenUpState* ALevelGenerator::_levelGenUpState = NewObject<ULevelGenUpState>();
ULevelGenLeftState* ALevelGenerator::_levelGenLeftState = NewObject<ULevelGenLeftState>();


// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


//#if WITH_EDITOR
//	// Load the asset registry module
//	FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
//	IAssetRegistry& assetRegistry = assetRegistryModule.Get();
//
//	TArray<FString> contentPaths;
//	contentPaths.Add("/LevelSegments/Segments");
//	assetRegistry.ScanFilesSynchronous(contentPaths);
//
//	FName baseClassName = ALevelSegment::StaticClass()->GetFName();
//
//	// Get all derived class names.
//	TSet<FName> derivedNames;
//	{
//		TArray<FName> baseNames;
//		baseNames.Add(baseClassName);
//
//		TSet<FName> excluded;
//		assetRegistry.GetDerivedClassNames(baseNames, excluded, derivedNames);
//	}
//
//	FARFilter filter;
//	filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
//	filter.bRecursiveClasses = true;
//	filter.PackagePaths.Add(TEXT("/LevelSegments/Segments"));
//	filter.bRecursivePaths = true;
//
//	TArray<FAssetData> assetList;
//	assetRegistry.GetAssets(filter, assetList);
//
//	
//	for (const FAssetData& asset : assetList)
//	{
//		if (const FString* generatedClassPathPtr = asset.TagsAndValues.Find(TEXT("GeneratedClass")))
//		{
//			// Convert path to just the name part.
//			const FString classObjectPath = FPackageName::ExportTextPathToObjectPath(*generatedClassPathPtr);
//			const FString className = FPackageName::ObjectPathToObjectName(classObjectPath);
//
//			// Check if the class is in the derived set.
//			if (!derivedNames.Contains(*className))
//			{
//				continue;
//			}
//
//			// Store using the path to the generated class.
//			
//		}
//	}
//#endif


	// Load all chunks, and store them according to their chunk descriptors.

}

//const TSet<FSegmentSpawnInfo>& ALevelGenerator::getValidRightSegments(const ESegmentTypes& segmentType) const
//{
//	return _validSegmentsLookup.Find(segmentType)->_validRightSegments;
//}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();


	_levelGenState = NewObject<ULevelGenUpState>();
	_levelGenState->init(this);


	GetWorldTimerManager().SetTimer(_timerHandle, this, &ALevelGenerator::updateLevelGen, 0.5f, true, 1.0f);
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ALevelGenerator::updateLevelGen()
{
	ULevelGenState* currLevelGenState = _levelGenState->updateState();

	// State has changed.
	if (currLevelGenState)
	{

	}
	// Still in previous state.
	else
	{
		_levelGenState->update();
	}
}
