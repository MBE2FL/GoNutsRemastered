// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Egg.h"
#include "ExplosionEgg.generated.h"


//class UMaterialInstanceConstant;


/**
 * 
 */
UCLASS()
class GONUTSREMASTERED_API AExplosionEgg : public AEgg
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AExplosionEgg();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;


	UFUNCTION()
	void onExplosionFinished(UParticleSystemComponent* system);
};



UCLASS()
class GONUTSREMASTERED_API AEggTarget : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEggTarget();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void setTargetColour(FLinearColor colour);

	UFUNCTION(BlueprintCallable)
	void setIsActive(bool isActive);


	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UMaterialInstanceDynamic* _targetMeshMat;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* _targetMeshComp;

	bool _isActive = false;
};