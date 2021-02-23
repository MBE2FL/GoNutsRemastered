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

void APlayerCharacter::stun()
{
	//ACharacter* parentChar = Cast<ACharacter>(GetRootComponent()->GetAttachParent()->GetOwner());
	UCharacterMovementComponent* parentMovementComp = _parentChar->GetCharacterMovement();


	parentMovementComp->Velocity = FVector::ZeroVector;
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

