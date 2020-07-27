// Fill out your copyright notice in the Description page of Project Settings.


#include "FreeRoamCharacter.h"
#include "Components/CapsuleComponent.h"
#include "LevelChunk.h"
#include "DrawDebugHelpers.h"
#include "LevelGenerator.h"

// Sets default values
AFreeRoamCharacter::AFreeRoamCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFreeRoamCharacter::init(ALevelGenerator* levelGen)
{
	if (IsValid(levelGen))
	{
		_levelGen = levelGen;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid level generator reference passed to the player!"));
	}
}

ALevelChunk* AFreeRoamCharacter::getChunk() const
{
	return _chunk;
}

// Called when the game starts or when spawned
void AFreeRoamCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	UCapsuleComponent* capsuleComp = GetCapsuleComponent();
	capsuleComp->OnComponentHit.AddDynamic(this, &AFreeRoamCharacter::OnCompHit);

}

// Called every frame
void AFreeRoamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Smoothly rotate this player.
	if (_rotatePlayer)
	{
		FQuat startRot = GetControlRotation().Quaternion();
		FQuat targetRot = _targetRot.Quaternion();

		_prevYaw = _newYaw;

		_rotateTValue = _rotateTValue + (10.0f * DeltaTime);

		if (_rotateTValue >= 1.0f)
		{
			_rotatePlayer = false;
			_rotateTValue = 0.0f;
			_newYaw = FQuat::Slerp(startRot, targetRot, 1.0f).Rotator().Yaw;
		}
		else
		{
			_newYaw = FQuat::Slerp(startRot, targetRot, _rotateTValue).Rotator().Yaw;
		}

		AddControllerYawInput(_newYaw - _prevYaw);
	}

	// Zero out pitch and roll.
	FRotator rotator = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	//FRotator rotator = FRotator(0.0f, GetActorRotation().Yaw, 0.0f);
	SetActorRotation(rotator);


	// Move this character forward.
	AddMovementInput(GetActorForwardVector(), 500.0f);
	//UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f, Z: %f"), forwardVector.X, forwardVector.Y, forwardVector.Z);



	// Find out which chunk is underneath this player.
	FHitResult hit;
	FCollisionQueryParams collisionParams;
	FVector startPos = GetActorLocation();
	startPos.Z -= GetMesh()->Bounds.BoxExtent.Z;
	FVector endPos = FVector(startPos.X, startPos.Y, startPos.Z - 20.0f);
	FColor colour = FColor::Red;


	if (GetWorld()->LineTraceSingleByChannel(hit, startPos, endPos, ECC_LevelChunkChannel, collisionParams))
	{
		if (hit.bBlockingHit)
		{
			colour = FColor::Green;

			ALevelChunk* chunk = Cast<ALevelChunk>(hit.GetActor());

			if (IsValid(chunk))
			{
				_chunk = chunk;
				//UE_LOG(LogTemp, Error, TEXT("Chunk underneath: %s"), *chunk->GetName());
			}
		}
	}

	DrawDebugLine(GetWorld(), startPos, endPos, colour, false, 1.0f, 0, 1.0f);
}

// Called to bind functionality to input
void AFreeRoamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AFreeRoamCharacter::startJump);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFreeRoamCharacter::moveRight);
	PlayerInputComponent->BindAction(TEXT("TurnLeft"), EInputEvent::IE_Pressed, this, &AFreeRoamCharacter::turnLeft);
	PlayerInputComponent->BindAction(TEXT("TurnRight"), EInputEvent::IE_Pressed, this, &AFreeRoamCharacter::turnRight);
}

void AFreeRoamCharacter::startJump()
{
	Jump();
}

void AFreeRoamCharacter::moveRight(float value)
{
	// Move left or right when the A or D key is pressed.
	AddMovementInput(GetActorRightVector(), value);
	AddMovementInput(GetActorForwardVector(), FMath::Abs(value));
}

void AFreeRoamCharacter::turnLeft()
{
	if (IsValid(_chunk) && IsValid(_levelGen))
	{
		if (_chunk->getChunckFeatures() & EChunkFeatures::CF_PLAYER_TURN_LEFT)
		{
			UE_LOG(LogTemp, Error, TEXT("Trying to go left!"));

			_levelGen->setMapOrientation(true);

			//SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
			//AddControllerYawInput(-90.0f);

			_rotatePlayer = true;
			_targetRot = GetActorRotation();
			_targetRot.Yaw -= 90.0f;
		}
	}
}

void AFreeRoamCharacter::turnRight()
{
	if (IsValid(_chunk) && IsValid(_levelGen))
	{
		if (_chunk->getChunckFeatures() & EChunkFeatures::CF_PLAYER_TURN_RIGHT)
		{
			UE_LOG(LogTemp, Error, TEXT("Trying to go right!"));

			_levelGen->setMapOrientation(false);

			//AddControllerYawInput(90.0f);

			_rotatePlayer = true;
			_targetRot = GetActorRotation();
			_targetRot.Yaw += 90.0f;
		}
	}
}

void AFreeRoamCharacter::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	//UE_LOG(LogTemp, Error, TEXT("COLLIDED: %s"), *OtherActor->GetName());
	//ALevelChunk* chunk = Cast<ALevelChunk>(OtherActor);

	//if (IsValid(chunk))
	//{
	//	_chunk = chunk;
	//}
}
