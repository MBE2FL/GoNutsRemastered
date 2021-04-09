// Fill out your copyright notice in the Description page of Project Settings.


#include "EmptyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AEmptyCharacter::AEmptyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = GetCapsuleComponent();
}

// Called when the game starts or when spawned
void AEmptyCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	// Retrieve the player's character.
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	if (!IsValid(playerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get player controller!"));
		return;
	}

	_playerChar = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (!IsValid(_playerChar))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get player character!"));
		return;
	}
	else
	{
		_playerChar->_onUpdateWalkSpeed.AddDynamic(this, &AEmptyCharacter::updateWalkSpeed);
	}


	GetCharacterMovement()->Velocity = FVector(600.0f, 0.0f, 0.0f);
}

// Called every frame
void AEmptyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (_playerChar->canPlayerMove())
	{
		// Zero out pitch and roll.
		FRotator rot = GetControlRotation();
		rot.Roll = 0.0f;
		rot.Pitch = 0.0f;

		AddMovementInput(UKismetMathLibrary::GetForwardVector(rot), _speedMultiplier);
	}
}

// Called to bind functionality to input
void AEmptyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEmptyCharacter::updateWalkSpeed(float walkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}
