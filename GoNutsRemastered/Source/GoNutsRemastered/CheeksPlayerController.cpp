// Fill out your copyright notice in the Description page of Project Settings.


#include "CheeksPlayerController.h"

ACheeksPlayerController::ACheeksPlayerController()
	: APlayerController()
{
	//InputPitchScale = 1.0f;
	//InputYawScale = 1.0f;
	//InputRollScale = 1.0f;
}

void ACheeksPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InputPitchScale = 1.0f;
	InputYawScale = 1.0f;
	InputRollScale = 1.0f;
}
