// Fill out your copyright notice in the Description page of Project Settings.


#include "EggEffect.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEggEffect::AEggEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creat an effect particle system.
	_effectParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect Particles"));
	_effectParticleComp->SetAutoActivate(false);
	_effectParticleComp->SetupAttachment(nullptr);
	RootComponent = _effectParticleComp;
}

// Called when the game starts or when spawned
void AEggEffect::BeginPlay()
{
	Super::BeginPlay();

	// Find the player's controller/actor.
	//FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator();
	//for (; it; ++it)
	//{
	//	_playerController = (*it).Get();

	//	if (_playerController)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Found Player Controller!"));
	//		break;
	//	}
	//}
	_playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	// Activate effect particle system.
	_effectParticleComp->ActivateSystem();

	// Start effect begin event.
	onEggEffectBegin();


}

bool AEggEffect::isPlayerWithinRange(const float range, float& dist, bool calcRealDist)
{
	if (!_playerPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("Player within range error: Player's pawn is not assigned!"));
		return false;
	}
	else
	{
		if (calcRealDist)
		{
			dist = FVector::Dist(_playerPawn->GetActorLocation(), GetActorLocation());
			return  dist <= range;
		}
		else
		{
			dist = FVector::DistSquared(_playerPawn->GetActorLocation(), GetActorLocation());
			return  dist <= FMath::Square(range);
		}
		
	}
}

// Called every frame
void AEggEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

