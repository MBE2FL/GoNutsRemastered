// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Egg.h"
#include "BirdmanCharacter.generated.h"

class UCharacterMovementComponent;

UCLASS()
class GONUTSREMASTERED_API ABirdmanCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABirdmanCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	UFUNCTION(BlueprintCallable)
	void updateWalkSpeed(float walkSpeed);

	UFUNCTION(BlueprintCallable)
	EEggType pickNextEgg();

	UFUNCTION(BlueprintCallable)
	void dropEgg();

	//UFUNCTION(BlueprintCallable)
	//void pickNextLane();

	//UFUNCTION(BlueprintCallable)
	//void moveToTargetLane();

	UFUNCTION(BlueprintCallable)
	bool decideToPursue();

	UFUNCTION(BlueprintImplementableEvent)
	void onEngagePursuit();

	UFUNCTION(BlueprintImplementableEvent)
	void onPursuitCooldownOver();

private:
	UPROPERTY()
	UCharacterMovementComponent* _movementComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FVector _stalkOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EEggType _currentEggType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TMap<EEggType, TSubclassOf<AEgg>> _eggTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool _isInPursuit = false;
	bool _prevTemp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float _pursuitCooldown = 2.0f;
	FTimerHandle _pursuitCooldownTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool _canPursue = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, ClampMin = 0.0f, ClampMax = 100.0f))
	float _engagePursuitChance = 25.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 _currentLane;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float _currentLaneY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 _targetLane;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float _targetLaneY;
};
