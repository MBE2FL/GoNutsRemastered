// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionEgg.h"
#include "Particles/ParticleSystemComponent.h"




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

	}
	// Overlapped with the player.
	else if (OtherComp->GetCollisionObjectType() == ECC_PlayerChannel)
	{
		//Cast<APlayerCharacter>(OtherActor)->stun();


		// Play death stuff.

		//Destroy();


		//_eggMeshComp->SetActive(false);
		_eggMeshComp->SetVisibility(false);
		_impactParticleComponent->ActivateSystem();
		
	}
}

void AExplosionEgg::onExplosionFinished(UParticleSystemComponent* system)
{
	Destroy();
}
