// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EggEffect.h"
#include "NapalmEggEffect.generated.h"

/**
 * 
 */
UCLASS()
class GONUTSREMASTERED_API ANapalmEggEffect : public AEggEffect
{
	GENERATED_BODY()

public:
	ANapalmEggEffect();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void activateEffect();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 100.0f, AllowPrivateAccess = true))
	float _damagePerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 50.0f, ClampMax = 2000.0f, AllowPrivateAccess = true))
	float _areaOfEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 2.0f, ClampMax = 20.0f, AllowPrivateAccess = true))
	float _effectDuration;
};
