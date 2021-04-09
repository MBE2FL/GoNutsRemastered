// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdmanCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Egg.h"
#include "ExplosionEgg.h"


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
	

	GetWorld()->GetTimerManager().SetTimer(
		_pursuitCooldownTimerHandle, [&]() 
		{ 
		_canPursue = true; 
		onPursuitCooldownOver();
		//GetWorld()->GetTimerManager().PauseTimer(_pursuitCooldownTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Timer!"));
		}, 
		_pursuitCooldown,
		false);



	_eggTarget->setIsActive(false);
}

void ABirdmanCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(_pursuitCooldownTimerHandle);
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


	FVector pos = _eggTarget->GetActorLocation();
	pos.Y = GetActorLocation().Y;
	_eggTarget->SetActorLocation(pos);
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
	int32 randomNum = FMath::RandRange(0, 100);

	if (randomNum >= 0 && randomNum < 50)
	{
		_currentEggType = EEggType::ROLL_EGG;
	}
	else
	{
		_currentEggType = EEggType::EXPLOSION_EGG;
		_eggTarget->setIsActive(true);
		_eggTarget->setTargetColour(FLinearColor::Black);
	}

	return _currentEggType;
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
		const FVector pos = GetActorLocation() - FVector(0.0f, 0.0f, 200.0f);
		const FRotator rot = GetActorRotation();
		AEgg* egg = Cast<AEgg>(GetWorld()->SpawnActor(_eggTypes[EEggType::ROLL_EGG].Get(), &pos, &rot));
	}
		break;
	case EEggType::GAS_EGG:
		break;
	case EEggType::GOO_EGG:
		break;
	case EEggType::EXPLOSION_EGG:
	{
		UE_LOG(LogTemp, Warning, TEXT("Dropped Explosion Egg!"));
		const FVector pos = GetActorLocation() - FVector(0.0f, 0.0f, 200.0f);
		const FRotator rot = GetActorRotation();
		AEgg* egg = Cast<AEgg>(GetWorld()->SpawnActor(_eggTypes[EEggType::EXPLOSION_EGG].Get(), &pos, &rot));
		egg->AttachToActor(GetAttachParentActor(), FAttachmentTransformRules::KeepWorldTransform);
		//egg->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		//egg->GetRootComponent()->AttachToComponent(GetAttachParentActor()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

		//_eggTarget->setTargetColour(FLinearColor::Red);
	}
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


		//FTimerDelegate timerCallback;
		//timerCallback.BindLambda([&]() { _canPursue = true; });
		//GetWorld()->GetTimerManager().SetTimer(_pursuitCooldownTimerHandle, timerCallback, _pursuitCooldown, false);
		//GetWorld()->GetTimerManager().SetTimer(
		//	_pursuitCooldownTimerHandle, 
		//	[&]() { _canPursue = true; onPursuitCooldownOver(); }, 
		//	_pursuitCooldown, 
		//	false);

		//UE_LOG(LogTemp, Warning, TEXT("Time Remaining: %f"), GetWorld()->GetTimerManager().GetTimerRemaining(_pursuitCooldownTimerHandle));
		//GetWorld()->GetTimerManager().UnPauseTimer(_pursuitCooldownTimerHandle);


		GetWorld()->GetTimerManager().ClearTimer(_pursuitCooldownTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			_pursuitCooldownTimerHandle, [&]()
			{
				_canPursue = true;
				onPursuitCooldownOver();
				UE_LOG(LogTemp, Warning, TEXT("Timer!"));
			},
			_pursuitCooldown,
			false);


		_eggTarget->setIsActive(false);
	}
}

bool ABirdmanCharacter::decideToPursue()
{
	float chance = FMath::FRandRange(0.0f, 100.0f);

	if (chance <= _engagePursuitChance)
	{
		_isInPursuit = true;
		_prevTemp = _isInPursuit;
		onEngagePursuit();

		UE_LOG(LogTemp, Warning, TEXT("Chance %f <= %f PChance"), chance, _engagePursuitChance);

		return true;
	}

	return false;
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

