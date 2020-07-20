// Fill out your copyright notice in the Description page of Project Settings.


#include "FreeRoamCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFreeRoamCharacter::AFreeRoamCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFreeRoamCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFreeRoamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Zero out pitch and roll.
	FRotator rotator = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	SetActorRotation(rotator);

	FVector forwardVector = GetActorForwardVector();

	// Move this character forward.
	AddMovementInput(forwardVector);
	//UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f, Z: %f"), forwardVector.X, forwardVector.Y, forwardVector.Z);
}

// Called to bind functionality to input
void AFreeRoamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AFreeRoamCharacter::startJump);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFreeRoamCharacter::moveRight);
}

void AFreeRoamCharacter::startJump()
{
	Jump();
}

void AFreeRoamCharacter::moveRight(float value)
{
	// Move left or right when the A or D key is pressed.
	FVector forwardVector = GetActorForwardVector();
	forwardVector.X = 0.0f;
	forwardVector.Y += value;
	forwardVector.Z = 0.0f;
	AddMovementInput(forwardVector);
}

