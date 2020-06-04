// Fill out your copyright notice in the Description page of Project Settings.


#include "NapalmEggEffect.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "Particles/ParticleSystemComponent.h"

ANapalmEggEffect::ANapalmEggEffect()
{
	// Set default values.
	static ConstructorHelpers::FObjectFinder<UParticleSystem> fireParticleSystem(TEXT("ParticleSystem'/Game/MobileStarterContent/Particles/P_Fire.P_Fire'"));
	_effectParticleComp->Template = fireParticleSystem.Object;

	_damagePerSecond = 10.0f;
	_areaOfEffect = 400.0f;
	_effectDuration = 4.0f;
}

void ANapalmEggEffect::BeginPlay()
{
	Super::BeginPlay();
}

void ANapalmEggEffect::Tick(float DeltaTime)
{
	// Deal damage to certain actors on specified area for as long as desired.
	if (_effectDuration > 0.0f)
	{
		float dist;

		if (isPlayerWithinRange(_areaOfEffect, dist))
		{
			TArray<AActor*> ignoredActors;

			UGameplayStatics::ApplyRadialDamage(GetWorld(), _damagePerSecond * DeltaTime, GetActorLocation(), _areaOfEffect, UDamageType::StaticClass(),
				ignoredActors, this, nullptr, false, ECC_Visibility);
		}

		_effectDuration -= DeltaTime;
	}
	// Destroy this effect once it has completed.
	else
	{
		Destroy();
	}
}

void ANapalmEggEffect::activateEffect()
{


}


