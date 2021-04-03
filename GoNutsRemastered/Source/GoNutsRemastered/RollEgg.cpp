// Fill out your copyright notice in the Description page of Project Settings.


#include "RollEgg.h"
#include "Curves/CurveFloat.h"
#include "PlayerCharacter.h"


//#define ECC_TileChannel ECollisionChannel::ECC_GameTraceChannel3
//#define ECC_PlayerChannel ECollisionChannel::ECC_GameTraceChannel4


ARollEgg::ARollEgg()
{
	_eggType = EEggType::ROLL_EGG;
}

void ARollEgg::BeginPlay()
{
	Super::BeginPlay();



	// Setup overlap callbacks.
	if (!_eggMeshComp)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Egg mesh component!"));
	}
	else
	{
		_eggMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ARollEgg::OnOverlapBegin);
		_eggMeshComp->OnComponentEndOverlap.AddDynamic(this, &ARollEgg::OnOverlapEnd);
	}
}

void ARollEgg::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// This egg is rolling.
	if (_rolling)
	{
		// Set the velocity of this egg according to the given curve and multipliers.
		_xVelTimer += DeltaTime;

		_xVelTimer = FMath::Clamp(_xVelTimer, 0.0f, _xVelMaxTime) / _xVelMaxTime;

		FVector pos = GetActorLocation();
		pos.X -= _xVelocity->GetFloatValue(_xVelTimer) * 20.0f;

		SetActorLocation(pos);
	}
}

void ARollEgg::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlapped: %s"), *OtherActor->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("Object Type: %i"), OtherComp->GetCollisionObjectType());


	// Overlapped with a tile.
	if (!_rolling && OtherComp->GetCollisionObjectType() == ECC_TileChannel)
	{
		UE_LOG(LogTemp, Warning, TEXT("Roll Egg Landed!"));

		_rolling = true;

		_eggMeshComp->SetSimulatePhysics(false);
	}
	// Overlapped with the player.
	else if (OtherComp->GetCollisionObjectType() == ECC_PlayerChannel)
	{
		Cast<APlayerCharacter>(OtherActor)->stun();


		// Play death stuff.

		Destroy();
	}
}
