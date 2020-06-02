// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GlideTrigger.generated.h"

UCLASS()
class GONUTSREMASTERED_API AGlideTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlideTrigger();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void VaultUp(AActor* otherActor, float dt, float tValue);

	UFUNCTION()
		void Glide(AActor* otherActor, float dt, float tValue);

	UPROPERTY(VisibleAnywhere)
		FColor _debugColour = FColor::Purple;

private:

	UPROPERTY(EditAnywhere, Category = "GlideTrigger")
		UStaticMeshComponent* _GlideTriggerMeshComp;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = true))
		float _targetPos;	

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = true))
		float _tValue;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = true))
		FVector _overlapLocation;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = true))
		FVector _glideDownLocation;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = true))
		bool _glideTriggerActive = false;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = true))
		bool _glideDown = false;

	UPROPERTY(EditAnywhere)
		AActor *_otherActor;

	UPROPERTY(EditAnywhere)
		UPrimitiveComponent* _otherMeshComp;

	UPROPERTY(EditAnywhere)
		UCharacterMovementComponent* _playerMovement;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = true))
		float _traceValue;
};
