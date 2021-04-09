// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EmptyCharacter.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	_parentChar = Cast<AEmptyCharacter>(GetAttachParentActor());
	_parentMovementComp = _parentChar->GetCharacterMovement();
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(_stopMovingTimerHandle);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// Regain velocity after getting hit by a rolling egg.
	if (_regainVelocity)
	{
		_regainVelTimer += DeltaTime;
		float tValue = _regainVelTimer / _regainVelTime;

		if (tValue >= 1.0f)
		{
			tValue = 1.0f;
			_regainVelocity = false;
			_regainVelTimer = 0.0f;
		}

		_parentMovementComp->Velocity.X = FMath::Lerp(_parentMovementComp->Velocity.X, _originalVelocity, tValue);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::updateWalkSpeed(float walkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	_onUpdateWalkSpeed.Broadcast(walkSpeed);
}

void APlayerCharacter::slowDown()
{
	onAdjustNutCount(-5);
	_originalVelocity = _parentMovementComp->Velocity.X;
	_parentMovementComp->Velocity.X *= _loseVelMultiplier;
	_regainVelocity = true;

	_regainVelTimer = 0.0f;
}

void APlayerCharacter::stun()
{
	//ACharacter* parentChar = Cast<ACharacter>(GetRootComponent()->GetAttachParent()->GetOwner());
	//UCharacterMovementComponent* parentMovementComp = _parentChar->GetCharacterMovement();


	_parentMovementComp->Velocity = FVector::ZeroVector;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	//updateWalkSpeed(0.0f);

	_canMove = false;

	GetWorld()->GetTimerManager().SetTimer(_stopMovingTimerHandle, this, &APlayerCharacter::unstun, _stunTime, false);

	// TO-DO: Play stun animation.
}

void APlayerCharacter::unstun()
{
	//updateWalkSpeed(600.0f);

	_canMove = true;

	// TO-DO: Stop stun animation.
}

void APlayerCharacter::addImpulse(const FVector& impulse)
{
	// Add X impluse to the parent character only.
	UCharacterMovementComponent* parentMovementComp = _parentChar->GetCharacterMovement();
	parentMovementComp->AddImpulse(FVector(impulse.X, 0.0f, 0.0f), true);
	parentMovementComp->Velocity.X = FMath::Clamp(parentMovementComp->Velocity.X, 600.0f, 1600.0f);


	// Add Y and Z impulse to the player character only.
	Cast< UCharacterMovementComponent>(GetMovementComponent())->AddImpulse(FVector(0.0f, impulse.Y, impulse.Z), true);
}

