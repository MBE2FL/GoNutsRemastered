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
	_mesh->SetCollisionProfileName(TEXT("Custom"));
	_mesh->SetCollisionObjectType(ECC_LevelSegmentChannel);
	_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
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

	_topDir = CreateDefaultSubobject<UArrowComponent>(TEXT("UpDir"));
	_topDir->SetupAttachment(_mesh);
	_topDir->SetRelativeLocationAndRotation(FVector(_mesh->Bounds.BoxExtent.X * 2.0f, 0.0f, 0.0f), FRotator::ZeroRotator);
	_topDir->SetArrowColor(FLinearColor::Red);
	_topDir->SetRelativeScale3D(FVector(3.0f, 3.0f, 1.0f));

	_bottomDir = CreateDefaultSubobject<UArrowComponent>(TEXT("DownDir"));
	_bottomDir->SetupAttachment(_mesh);
	_bottomDir->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 180.0f, 0.0f));
	_bottomDir->SetArrowColor(FLinearColor::Black);
	_bottomDir->SetRelativeScale3D(FVector(3.0f, 3.0f, 1.0f));	
}

ESegmentTypes ALevelSegment::getSegmentType() const
{
	return _segmentType;
}

ESegmentFeatures::Type ALevelSegment::getSegmentFeatures() const
{
	return static_cast<ESegmentFeatures::Type>(_segmentFeatures);
}

void ALevelSegment::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{

}

UStaticMeshComponent* ALevelSegment::getMesh() const
{
	return _mesh;
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

	float hOffset = 0.0f;

	switch (_orientation)
	{
	case ESegmentOrientations::SEGO_Right:
		hOffset = GetActorLocation().Y + (getMesh()->Bounds.BoxExtent.Y * 2.0f);
		break;
	case ESegmentOrientations::SEGO_Left:
		hOffset = GetActorLocation().Y;
		break;
	case ESegmentOrientations::SEGO_Up:
		hOffset = GetActorLocation().Y + (getMesh()->Bounds.BoxExtent.Y * 2.0f);
		break;
	case ESegmentOrientations::SEGO_Down:
		hOffset = GetActorLocation().Y + (getMesh()->Bounds.BoxExtent.Y * 2.0f);
		break;
	default:
		break;
	}

	return hOffset;
}

FVector ALevelSegment::getHOffsetLocation() const
{
	FVector hOffsetLocation;

	switch (_orientation)
	{
	case ESegmentOrientations::SEGO_Right:
		hOffsetLocation = GetActorLocation();
		hOffsetLocation.Y += (getMesh()->Bounds.BoxExtent.Y * 2.0f);
		break;
	case ESegmentOrientations::SEGO_Left:
		hOffsetLocation = GetActorLocation();
		break;
	case ESegmentOrientations::SEGO_Up:
		hOffsetLocation = GetActorLocation();
		hOffsetLocation.Y += (getMesh()->Bounds.BoxExtent.Y * 2.0f);
		break;
	case ESegmentOrientations::SEGO_Down:
		hOffsetLocation = GetActorLocation();
		hOffsetLocation.Y += (getMesh()->Bounds.BoxExtent.Y * 2.0f);
		break;
	default:
		break;
	}

	return hOffsetLocation;
}

float ALevelSegment::getLocalVOffset() const
{
	float vOffset = 0.0f;

	switch (_orientation)
	{
	case ESegmentOrientations::SEGO_Right:
		vOffset = 0.0f;
		break;
	case ESegmentOrientations::SEGO_Left:
		vOffset = getMesh()->Bounds.BoxExtent.X * 2.0f;
		break;
	case ESegmentOrientations::SEGO_Up:
		vOffset = 0.0f;
		break;
	case ESegmentOrientations::SEGO_Down:
		vOffset = getMesh()->Bounds.BoxExtent.X * 2.0f;
		break;
	default:
		break;
	}

	return vOffset;
}

float ALevelSegment::getWorldVOffset() const
{
	float vOffset = 0.0f;

	switch (_orientation)
	{
	case ESegmentOrientations::SEGO_Right:
		vOffset = GetActorLocation().X;
		break;
	case ESegmentOrientations::SEGO_Left:
		vOffset = GetActorLocation().X - (getMesh()->Bounds.BoxExtent.X * 2.0f);
		break;
	case ESegmentOrientations::SEGO_Up:
		vOffset = GetActorLocation().X;
		break;
	case ESegmentOrientations::SEGO_Down:
		vOffset = GetActorLocation().X - (getMesh()->Bounds.BoxExtent.X * 2.0f);
		break;
	default:
		break;
	}

	//vOffset = GetActorLocation().X;
	return vOffset;
}

ESegmentOrientations ALevelSegment::getOrientation() const
{
	return _orientation;
}

void ALevelSegment::setOrientation(const ESegmentOrientations orientation)
{
	_orientation = orientation;
}

void ALevelSegment::setSpawnPoints(UPARAM(ref)TArray<USceneComponent*> carSpawnPoints, UPARAM(ref)TArray<USceneComponent*> obstableSpawnPoints, UPARAM(ref)TArray<USceneComponent*> pedestrianSpawnPoints)
{
	_carSpawnPoints = carSpawnPoints;
	_obstacleSpawnPoints = obstableSpawnPoints;
	_pedestrianSpawnPoints = pedestrianSpawnPoints;
}

const TArray<USceneComponent*>& ALevelSegment::getObstacleSpawnPoints() const
{
	return _obstacleSpawnPoints;
}

const TArray<USceneComponent*>& ALevelSegment::getPedestrianSpawnPoints() const
{
	return _pedestrianSpawnPoints;
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
	return _validTopSegments;
}

const TSet<FSegmentSpawnInfo>& ALevelSegment::getValidDownSegments() const
{
	return _validBottomSegments;
}

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
