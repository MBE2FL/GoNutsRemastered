// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomCharacter.generated.h"

class ALevelChunk;
class ALevelGenerator;
class UPlayerMoveState;
class UPlayerDefaultMoveState;
class UPlayerTurningMoveState;
class UPlayerAltLaneMoveState;

DECLARE_LOG_CATEGORY_EXTERN(LogCustomCharacter, Log, All);

UCLASS(Abstract)
class GONUTSREMASTERED_API ACustomCharacter : public ACharacter
{
	GENERATED_BODY()

	friend class UPlayerMoveState;
	friend class UPlayerDefaultMoveState;
	friend class UPlayerTurningMoveState;
	friend class UPlayerAltLaneMoveState;

public:
	// Sets default values for this character's properties
	ACustomCharacter();
	void init(ALevelGenerator* levelGen);
	ALevelChunk* getChunk() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void intersectionTransition() {};

	FORCEINLINE bool getIsRotating() const;
	
protected:
	virtual void startJump() {};
	virtual void turnLeft() {};
	virtual void turnRight() {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	ALevelChunk* _chunk = nullptr;

	UPROPERTY()
	ALevelGenerator* _levelGen = nullptr;


	// TO-DO Change to timeline.
	UPROPERTY()
	bool _rotatePlayer = false;
	UPROPERTY()
	FRotator _targetRot;
	UPROPERTY()
	float _rotateTValue = 0.0f;
	UPROPERTY()
	float _prevYaw = 0.0f;
	UPROPERTY()
	float _newYaw = 0.0f;
};
