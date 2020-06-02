// Fill out your copyright notice in the Description page of Project Settings.


#include "GlideTrigger.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGlideTrigger::AGlideTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_GlideTriggerMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Test"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	_GlideTriggerMeshComp->SetStaticMesh(meshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> matAsset(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	_GlideTriggerMeshComp->SetMaterial(0, matAsset.Object);
	_GlideTriggerMeshComp->SetCollisionProfileName(TEXT("Custom"));
	_GlideTriggerMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	_GlideTriggerMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	_GlideTriggerMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	_GlideTriggerMeshComp->SetupAttachment(nullptr);
	RootComponent = _GlideTriggerMeshComp;

}

// Called when the game starts or when spawned
void AGlideTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	if (!_GlideTriggerMeshComp)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get GlideTrigger Cube component!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Got GlideTrigger Cube component!"));
		_GlideTriggerMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AGlideTrigger::OnOverlapBegin);
		_GlideTriggerMeshComp->OnComponentEndOverlap.AddDynamic(this, &AGlideTrigger::OnOverlapEnd);
	}
}

void AGlideTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Glide trigger collide with player!"));
	_debugColour = FColor::Purple;
	_glideTriggerActive = true;
	_otherActor = OtherActor;
	_playerMovement = Cast<UCharacterMovementComponent>(_otherActor->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	_playerMovement->GravityScale = 0.0f;
	//_overlapLocation = OtherActor->GetActorLocation();
	
}

void AGlideTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("collision stopped with player"));
	_debugColour = FColor::Purple;
	
}

void AGlideTrigger::VaultUp(AActor* otherActor, float dt, float tValue)
{
	FVector temp;
	temp.X = otherActor->GetActorLocation().X;
	temp.Y = otherActor->GetActorLocation().Y;
	temp.Z = FMath::FInterpTo(_otherActor->GetActorLocation().Z, 1000.0f, dt, tValue);

	if (temp.Z  > 990.0f)
	{
		_glideTriggerActive = false;
		_glideDown = true;
		_tValue = 0;
		_glideDownLocation = otherActor->GetActorLocation();
	}
	else
	{
		otherActor->SetActorLocation(temp);
		//UE_LOG(LogTemp, Warning, TEXT(temp.ToString()));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My Location is: %s"), *temp.ToString()));
	}
}

void AGlideTrigger::Glide(AActor* otherActor, float dt, float tValue)
{
	FVector traceLoc = otherActor->GetActorLocation();
	FVector traceEnd = FVector(0,0,-300.0f);
	FVector traceRot = FVector(0,270.0f,0);
	FHitResult traceHit;
	FCollisionQueryParams traceParams;

	//GetWorld()->LineTraceSingleByChannel(traceHit, traceLoc, traceEnd, ECC_Visibility, traceParams);
	//DrawDebugLine(GetWorld(), traceLoc, traceEnd, FColor::Orange, true);

	FVector temp;
	temp.X = otherActor->GetActorLocation().X;
	temp.Y = otherActor->GetActorLocation().Y;
	temp.Z = FMath::FInterpTo(_otherActor->GetActorLocation().Z, 100.0f, dt, tValue);

	if (temp.Z < 301.0f/*GetWorld()->LineTraceSingleByChannel(traceHit, traceLoc, traceEnd, ECC_Visibility, traceParams)*/)
	{
		_glideDown = false;
		_playerMovement->GravityScale = 1.0f;
		_tValue = 0;
	}
	else
	{
		otherActor->SetActorLocation(temp);
	}
}

// Called every frame
void AGlideTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_glideTriggerActive)
	{
		//_tValue += DeltaTime * 0.05f;
		//if (_tValue > 1.0f)
		//	_tValue = 1.0f;

		VaultUp(_otherActor, DeltaTime, 1.0f);
	}
	else if (_glideDown)
	{
		//_tValue += DeltaTime * 0.1f;
		//if (_tValue > 1.0f)
		//	_tValue = 1.0f;

		Glide(_otherActor, DeltaTime, 0.2f);
	}
}

