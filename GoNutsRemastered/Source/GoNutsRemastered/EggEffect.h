// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EggEffect.generated.h"

class UParticleSystemComponent;

//UDELEGATE()
//DECLARE_EVENT(AEggEffect, FEggEffectEvent)

//UDELEGATE(BlueprintAuthorityOnly)
//DECLARE_DELEGATE(FEggDelegate)

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEggEffectSignature, float, paramOne);


UCLASS(Abstract, Blueprintable)
class GONUTSREMASTERED_API AEggEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEggEffect();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable)
	//FEggEffectEvent& onEggEffect() { return _eggEvent; };


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Egg Effect|Effect Settings")
	void onEggEffectBegin();

	UFUNCTION(BlueprintPure, Category = "Egg Effect|Utility")
	bool isPlayerWithinRange(const float range, float& dist, bool calcRealDist = false);


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "Egg Effect|Effect Settings")
	//FEggEffectDelegate _eggEffectDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "Egg Effect|Effect Settings")
	FEggEffectSignature _eggEffectSig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg Effect|Effect Settings")
	UParticleSystemComponent* _effectParticleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Egg Effect|General Settings")
	APawn* _playerPawn;

};
