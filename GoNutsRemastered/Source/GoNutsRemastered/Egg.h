// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Particles/ParticleSystem.h"

#include "Egg.generated.h"//Make sure this is at the bottom of the includes

class AEggEffect;
class UParticleSystemComponent;


#define ECC_TileChannel ECollisionChannel::ECC_GameTraceChannel3
#define ECC_PlayerChannel ECollisionChannel::ECC_GameTraceChannel4


UENUM(BlueprintType)
enum class EEggType : uint8
{
	FREEZE_EGG = 0,
	ROLL_EGG = 1,
	GAS_EGG = 2,
	GOO_EGG = 3,
	EXPLOSION_EGG = 4
};


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
	void onEggImpact(EEggType eggType, TSubclassOf<AEggEffect> eggEffectType, AEggEffect* eggEffectActor);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(VisibleAnywhere)
	FColor _debugColour = FColor::Purple;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "Egg|Effect Settings")
	FEggEffectDelegate _eggEffectDelegate;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* _eggMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg|Impact Settings", meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* _impactParticleComponent;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg|Effect Settings", meta = (AllowPrivateAccess = true))
	//AEggEffect* _eggEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg|Effect Settings", meta = (AllowPrivateAccess = true))
	TSubclassOf<AEggEffect> _eggEffectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg", meta = (AllowPrivateAccess = true))
	EEggType _eggType;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Egg")
	APawn* _playerPawn;
};
