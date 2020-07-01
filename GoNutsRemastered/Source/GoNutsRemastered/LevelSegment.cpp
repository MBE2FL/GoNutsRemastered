// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSegment.h"
#include "Components/ArrowComponent.h"

// Sets default values
ALevelSegment::ALevelSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	_mesh->SetStaticMesh(meshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> matAsset(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	_mesh->SetMaterial(0, matAsset.Object);
	RootComponent = _mesh;


	_rightDir = CreateDefaultSubobject<UArrowComponent>(TEXT("RightDir"));
	_rightDir->SetupAttachment(_mesh);
	_rightDir->SetRelativeLocationAndRotation(FVector(0.0f, _mesh->Bounds.BoxExtent.Y * 2.0f, 0.0f), FRotator(0.0f, 90.0f, 0.0f));
	_rightDir->SetArrowColor(FLinearColor::Green);
	_rightDir->SetRelativeScale3D(FVector(3.0f, 3.0f, 1.0f));

	_leftDir = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftDir"));
	_leftDir->SetupAttachment(_mesh);
	_leftDir->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));
	_leftDir->SetArrowColor(FLinearColor::Yellow);
	_leftDir->SetRelativeScale3D(FVector(3.0f, 3.0f, 1.0f));

	_upDir = CreateDefaultSubobject<UArrowComponent>(TEXT("UpDir"));
	_upDir->SetupAttachment(_mesh);
	_upDir->SetRelativeLocationAndRotation(FVector(_mesh->Bounds.BoxExtent.X * 2.0f, 0.0f, 0.0f), FRotator::ZeroRotator);
	_upDir->SetArrowColor(FLinearColor::Red);
	_upDir->SetRelativeScale3D(FVector(3.0f, 3.0f, 1.0f));

	_downDir = CreateDefaultSubobject<UArrowComponent>(TEXT("DownDir"));
	_downDir->SetupAttachment(_mesh);
	_downDir->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 180.0f, 0.0f));
	_downDir->SetArrowColor(FLinearColor::Black);
	_downDir->SetRelativeScale3D(FVector(3.0f, 3.0f, 1.0f));


	//_mesh->OnStaticMeshChanged().AddUObject(this, &ALevelSegment::onMeshChanged);
	
}

ESegmentTypes ALevelSegment::getSegmentType() const
{
	return _segmentType;
}

void ALevelSegment::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{

}

UStaticMeshComponent* ALevelSegment::getMesh() const
{
	return _mesh;
}

USceneComponent* ALevelSegment::getNextHSlot() const
{
	//return _nextHSlot;
	return nullptr;
}

float ALevelSegment::getHOffset() const
{
	//return _mesh->Bounds.BoxExtent.Y * 2.0f;

	//if (!_nextHSlot)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Level Gen Actor does not have the required horizontal slot!"));
	//	return 0.0f;
	//}
	//else
	//{
	//	return _nextHSlot->GetComponentLocation().Y;
	//}

	return 0.0f;
}

USceneComponent* ALevelSegment::getNextVSlot() const
{
	//return _nextVSlot;
	return nullptr;
}

ESegmentOrientations ALevelSegment::getOrientation() const
{
	return _orientation;
}

void ALevelSegment::setOrientation(const ESegmentOrientations orientation)
{
	_orientation = orientation;
}

const TSet<FSegmentSpawnInfo>& ALevelSegment::getValidRightSegments() const
{
	return _validRightSegments;
}

const TSet<FSegmentSpawnInfo>& ALevelSegment::getValidLeftSegments() const
{
	return _validLeftSegments;
}

const TSet<FSegmentSpawnInfo>& ALevelSegment::getValidUpSegments() const
{
	return _validUpSegments;
}

const TSet<FSegmentSpawnInfo>& ALevelSegment::getValidDownSegments() const
{
	return _validDownSegments;
}

//USceneComponent* ALevelSegment::getPrevHSlot() const
//{
//	return _prevHSlot;
//}
//
//USceneComponent* ALevelSegment::getPrevVSlot() const
//{
//	return _prevVSlot;
//}

// Called when the game starts or when spawned
void ALevelSegment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
