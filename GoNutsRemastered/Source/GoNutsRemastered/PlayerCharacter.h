// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateWalkSpeedDelegate, float, walkSpeed);


UCLASS()
class GONUTSREMASTERED_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	inline bool canPlayerMove() const
	{
		return _canMove;
	}

	UFUNCTION(BlueprintCallable)
	void updateWalkSpeed(float walkSpeed);

	UFUNCTION(BlueprintCallable)
	void stun();

	UFUNCTION(BlueprintCallable)
	void unstun();



	UPROPERTY(BlueprintAssignable)
	FOnUpdateWalkSpeedDelegate _onUpdateWalkSpeed;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool _canMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float _stunTime = 1.0f;

	FTimerHandle _stopMovingTimerHandle;
};
