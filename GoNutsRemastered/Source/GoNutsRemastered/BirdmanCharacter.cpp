// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdmanCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Egg.h"

// Sets default values
ABirdmanCharacter::ABirdmanCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_movementComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
}

// Called when the game starts or when spawned
void ABirdmanCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABirdmanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (_isInPursuit && (_prevTemp != _isInPursuit))
	{
		UE_LOG(LogTemp, Warning, TEXT("TEST!"));
		_prevTemp = _isInPursuit;

		onEngagePursuit();
	}
}

// Called to bind functionality to input
void ABirdmanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABirdmanCharacter::updateWalkSpeed(float walkSpeed)
{
	_movementComp->MaxWalkSpeed = walkSpeed;
}

EEggType ABirdmanCharacter::pickNextEgg()
{
	_currentEggType = EEggType::ROLL_EGG;

	return EEggType::ROLL_EGG;
}

void ABirdmanCharacter::dropEgg()
{
	switch (_currentEggType)
	{
	case EEggType::FREEZE_EGG:
		break;
	case EEggType::ROLL_EGG:
	{
		UE_LOG(LogTemp, Warning, TEXT("Dropped Roll Egg!"));
		const FVector pos = GetActorLocation();
		const FRotator rot = GetActorRotation();
		AEgg* egg = Cast<AEgg>(GetWorld()->SpawnActor(_eggTypes[EEggType::ROLL_EGG].Get(), &pos, &rot));
	}
		break;
	case EEggType::GAS_EGG:
		break;
	case EEggType::GOO_EGG:
		break;
	case EEggType::EXPLOSION_EGG:
		break;
	default:
		break;
	}


	bool shouldStopPursuing = FMath::RandRange(1, 10) <= 3;

	if (shouldStopPursuing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stopping Pursuit!"));
		_isInPursuit = false;
		_prevTemp = _isInPursuit;
		onEngagePursuit();

		_currentLane = 1;
		_currentLaneY = 0.0f;
	}
}

//void ABirdmanCharacter::pickNextLane()
//{
//	_currentLaneY = 
//	_targetLane = FMath::RandRange(-1, 1);
//}
//
//void ABirdmanCharacter::moveToTargetLane()
//{
//}

