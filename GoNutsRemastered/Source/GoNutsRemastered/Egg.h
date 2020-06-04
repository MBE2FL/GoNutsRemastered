// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Particles/ParticleSystem.h"

#include "Egg.generated.h"//Make sure this is at the bottom of the includes

class AEggEffect;
class UParticleSystemComponent;

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEggEffectDelegate, AEggEffect*, eggEffect);


UCLASS(Blueprintable)
class GONUTSREMASTERED_API AEgg : public AActor
{
	GENERATED_BODY()//Never Delete this!!!!!

public:	
	// Sets default values for this actor's properties
	AEgg();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Egg|Effect Settings")
	void onEggImpact(TSubclassOf<AEggEffect> eggEffectType, AEggEffect* eggEffectActor);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(VisibleAnywhere)
	FColor _debugColour = FColor::Purple;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "Egg|Effect Settings")
	FEggEffectDelegate _eggEffectDelegate;


private:
	UPROPERTY(EditAnywhere, Category = "Egg")
	UStaticMeshComponent* _eggMeshComp;

	UPROPERTY(EditAnywhere, Category = "Egg|Impact Settings")
	UParticleSystemComponent* _impactParticleComponent;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg|Effect Settings", meta = (AllowPrivateAccess = true))
	//AEggEffect* _eggEffect;

	UPROPERTY(EditAnywhere, Category = "Egg|Effect Settings")
	TSubclassOf<AEggEffect> _eggEffectType;
};
