// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacter.h"
#include "LaneCharacter.generated.h"


class ULaneComponent;
class UTimelineComponent;


DECLARE_LOG_CATEGORY_EXTERN(LogLaneCharacter, Log, All);


UENUM(BlueprintType)
enum class ELaneSwitchDirection : uint8
{
	LSD_NONE,
	LSD_LEFT,
	LSD_RIGHT
};

UENUM(BlueprintType)
enum class ETransitionDirection : uint8
{
	TD_NONE,
	TD_LEFT,
	TD_RIGHT
};


UCLASS()
class GONUTSREMASTERED_API ALaneCharacter : public ACustomCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALaneCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void intersectionTransition() override;


	static UPlayerDefaultMoveState* getPlayerDefaultMoveState() { return _defaultMoveState; };
	static UPlayerTurningMoveState* getPlayerTurningMoveState() { return _turningMoveState; };
	static UPlayerAltLaneMoveState* getPlayerAltLaneMoveState() { return _altLaneMoveState; };


private:
	// Movement functions
	virtual void startJump() override;
	void switchToLeftLane();
	void switchToRightLane();
	virtual void turnLeft() override;
	virtual void turnRight() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UPlayerMoveState* _currentMoveState = nullptr;

	static UPlayerDefaultMoveState* _defaultMoveState;
	static UPlayerTurningMoveState* _turningMoveState;
	static UPlayerAltLaneMoveState* _altLaneMoveState;

	//bool _chunkChanged = false;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	//ULaneComponent* _currentLane = nullptr;

	//UPROPERTY()
	//ULaneComponent* _lastOverlappedLane;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//UCurveVector* _swapLaneCurve;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//UTimelineComponent* _swapLaneTimeline;

	//UFUNCTION()
	//void swapLaneTimelineCallback(float position);
	//UFUNCTION()
	//void swapLaneTimelineFinishedCallback();



	//ELaneSwitchDirection _laneSwitchDirection = ELaneSwitchDirection::LSD_NONE;
	//float _targetY;
	//bool _moveToNewLane = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//float _laneSwitchSpeed = 18.0f;
	//ETransitionDirection _transitionDirection = ETransitionDirection::TD_NONE;
};
