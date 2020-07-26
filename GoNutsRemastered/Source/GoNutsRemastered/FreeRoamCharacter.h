// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FreeRoamCharacter.generated.h"

class ALevelChunk;
class ALevelGenerator;

UCLASS()
class GONUTSREMASTERED_API AFreeRoamCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFreeRoamCharacter();
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


private:
	void startJump();
	void moveRight(float value);
	void turnLeft();
	void turnRight();

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& HitResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	ALevelChunk* _chunk = nullptr;

	UPROPERTY()
	ALevelGenerator* _levelGen = nullptr;
};
