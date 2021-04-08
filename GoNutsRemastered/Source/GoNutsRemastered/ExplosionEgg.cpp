// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionEgg.h"
#include "Particles/ParticleSystemComponent.h"
//#include "Components/StaticMeshComponent.h"
//#include "Materials/Material.h"
//#include "Materials/MaterialInstanceConstant.h"
#include "DrawDebugHelpers.h"
#include "PlayerCharacter.h"



AExplosionEgg::AExplosionEgg()
{
	_eggType = EEggType::EXPLOSION_EGG;
}

void AExplosionEgg::BeginPlay()
{
	Super::BeginPlay();

	// Setup overlap callbacks.
	if (!IsValid(_eggMeshComp))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get egg mesh component!"));
	}
	else
	{
		_eggMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AExplosionEgg::OnOverlapBegin);
	}


	if (!IsValid(_impactParticleComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get impact particle system component!"));
	}
	else
	{
		_impactParticleComponent->OnSystemFinished.AddDynamic(this, &AExplosionEgg::onExplosionFinished);
	}
}

void AExplosionEgg::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!IsValid(_impactParticleComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get impact particle system component!"));
	}
	else
	{
		_impactParticleComponent->OnSystemFinished.RemoveDynamic(this, &AExplosionEgg::onExplosionFinished);
	}
}

void AExplosionEgg::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// Overlapped with a tile.
	if (OtherComp->GetCollisionObjectType() == ECC_TileChannel)
	{
		_eggMeshComp->SetSimulatePhysics(false);
		_eggMeshComp->SetVisibility(false);
		_eggMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetActorTickEnabled(false);
		_impactParticleComponent->ActivateSystem();
	}
	// Overlapped with the player.
	else if (OtherComp->GetCollisionObjectType() == ECC_PlayerChannel)
	{
		Cast<APlayerCharacter>(OtherActor)->onKillPlayer();


		// Play death stuff.

		//Destroy();


		//_eggMeshComp->SetActive(false);
		_eggMeshComp->SetSimulatePhysics(false);
		_eggMeshComp->SetVisibility(false);
		_eggMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetActorTickEnabled(false);
		_impactParticleComponent->ActivateSystem();
	}
}

void AExplosionEgg::onExplosionFinished(UParticleSystemComponent* system)
{
	Destroy();
}





AEggTarget::AEggTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	_targetMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	_targetMeshComp->SetStaticMesh(meshAsset.Object);
	//static ConstructorHelpers::FObjectFinder<UMaterial> matAsset(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> matAsset(TEXT("Material'/Game/Birdman/Eggs/Target_Mat.Target_Mat'"));
	_targetMeshComp->SetMaterial(0, matAsset.Object);


	_targetMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	//_targetMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	_targetMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	_targetMeshComp->SetupAttachment(nullptr);
	RootComponent = _targetMeshComp;
}

void AEggTarget::BeginPlay()
{
	Super::BeginPlay();

	//static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> matInstanceAsset(TEXT("MaterialInstanceConstant'/Game/Birdman/Eggs/Target_Mat_Inst.Target_Mat_Inst'"));
	UMaterialInstanceDynamic* targetMeshMat = UMaterialInstanceDynamic::Create(_targetMeshComp->GetMaterial(0), _targetMeshComp);
	//_targetMeshMat = matInstanceAsset.Object;
	targetMeshMat->SetVectorParameterValue(TEXT("TargetColour"), FLinearColor::Black);
	//_targetMeshMat->setVector
	_targetMeshComp->SetMaterial(0, targetMeshMat);
}

void AEggTarget::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	setTargetColour(FLinearColor::Black);
}

void AEggTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Error, TEXT("JLKSDJLFK"));


	if (_isActive)
	{
		//// Find out which chunk is underneath this player.
		//FHitResult hit;
		//FCollisionQueryParams collisionParams;
		//FVector startPos = GetActorLocation();
		//startPos.Z -= GetMesh()->Bounds.BoxExtent.Z;
		//FVector endPos = FVector(startPos.X, startPos.Y, startPos.Z - 20.0f);
		//FColor colour = FColor::Red;


		//if (GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, ECC_LevelChunkChannel, collisionParams))
		//{
		//	if (hit.bBlockingHit)
		//	{
		//		colour = FColor::Green;

		//		ALevelChunk* chunk = Cast<ALevelChunk>(hit.GetActor());

		//		if (IsValid(chunk))
		//		{
		//			_chunk = chunk;
		//			//UE_LOG(LogTemp, Error, TEXT("Chunk underneath: %s"), *chunk->GetName());
		//		}
		//	}
		//}

		//DrawDebugLine(GetWorld(), startPos, endPos, colour, false, 1.0f, 0, 1.0f);


		FHitResult hit;
		FCollisionObjectQueryParams objectParams(ECC_TO_BITFIELD(ECC_EggChannel));
		FCollisionQueryParams collisionParams;
		FVector startPos = GetActorLocation();
		FVector endPos = FVector(startPos.X, startPos.Y, startPos.Z + 600.0f);
		FColor colour = FColor::Green;

		//if (GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, ECC_EggChannel))
		if (GetWorld()->LineTraceSingleByObjectType(hit, startPos, endPos, objectParams))
		{
			setTargetColour(FLinearColor::Red);
			colour = FColor::Red;
		}
		else
		{
			setTargetColour(FLinearColor::Black);
		}

		DrawDebugLine(GetWorld(), startPos, endPos, colour, false, 0.1f, 0, 3.0f);
	}
}

void AEggTarget::setTargetColour(FLinearColor colour)
{
	Cast<UMaterialInstanceDynamic>(_targetMeshComp->GetMaterial(0))->SetVectorParameterValue(TEXT("TargetColour"), colour);
}

void AEggTarget::setIsActive(bool isActive)
{
	if (isActive)
	{
		SetActorHiddenInGame(false);
		// SetEnableCollision
		// SetActorTickEnabled
	}
	else
	{
		SetActorHiddenInGame(true);
	}

	_isActive = isActive;
}
