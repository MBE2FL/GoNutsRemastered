// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CheeksPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GONUTSREMASTERED_API ACheeksPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACheeksPlayerController();

protected:
	virtual void BeginPlay() override;
};
