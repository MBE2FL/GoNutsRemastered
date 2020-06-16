// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PedestrianManager.generated.h"

class ACharacter;
class ALevelSegment;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GONUTSREMASTERED_API UPedestrianManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPedestrianManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pedestrian Manager|Pedestrian Types", meta = (AllowPrivateAccess = true))
	TArray<ACharacter*> _pedestrianTypes;

	UFUNCTION()
	void spawnPedestrians(ALevelSegment* crosswalk);
};
