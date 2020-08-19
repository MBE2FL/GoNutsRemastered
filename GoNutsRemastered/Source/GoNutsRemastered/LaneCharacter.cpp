// Fill out your copyright notice in the Description page of Project Settings.


#include "LaneCharacter.h"
#include "LevelChunk.h"
#include "IntersectionChunk.h"
#include "LevelGenerator.h"
#include "LaneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveVector.h"
#include "Components/CapsuleComponent.h"
#include "PlayerMoveState.h"
#include "PlayerDefaultMoveState.h"
#include "PlayerTurningMoveState.h"
#include "PlayerAltLaneMoveState.h"
#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif

DEFINE_LOG_CATEGORY(LogLaneCharacter);

UPlayerDefaultMoveState* ALaneCharacter::_defaultMoveState = nullptr;
UPlayerTurningMoveState* ALaneCharacter::_turningMoveState = nullptr;
UPlayerAltLaneMoveState* ALaneCharacter::_altLaneMoveState = nullptr;

// Sets default values
ALaneCharacter::ALaneCharacter()
	: ACustomCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALaneCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UBoxComponent* boxComp = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ALaneCharacter::OnBeginOverlap);


	//// Swap lane timeline.
	//if (IsValid(_swapLaneCurve))
	//{
	//	_swapLaneTimeline = NewObject<UTimelineComponent>(this, TEXT("SwapLaneTimeline"));

	//	FOnTimelineVector timelineCallback;
	//	FOnTimelineEventStatic timelineFinishedCallback;

	//	timelineCallback.BindUFunction(this, TEXT("swapLaneTimelineCallback"));
	//	timelineFinishedCallback.BindUFunction(this, TEXT("swapLaneTimelineFinishedCallback"));
	//	_swapLaneTimeline->AddInterpVector(_swapLaneCurve, timelineCallback);
	//	_swapLaneTimeline->SetTimelineFinishedFunc(timelineFinishedCallback);

	//	_swapLaneTimeline->RegisterComponent();
	//}


	_defaultMoveState = NewObject<UPlayerDefaultMoveState>();
	_defaultMoveState->AddToRoot();
	_defaultMoveState->init(this);
	_currentMoveState = _defaultMoveState;

	_turningMoveState = NewObject<UPlayerTurningMoveState>();
	_turningMoveState->AddToRoot();
	_turningMoveState->init(this);

	_altLaneMoveState = NewObject<UPlayerAltLaneMoveState>();
	_altLaneMoveState->AddToRoot();
	_altLaneMoveState->init(this);
}

void ALaneCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(_defaultMoveState))
	{
		_defaultMoveState->ConditionalBeginDestroy();
		_defaultMoveState = nullptr;
	}

	if (IsValid(_turningMoveState))
	{
		_turningMoveState->ConditionalBeginDestroy();
		_turningMoveState = nullptr;
	}

	if (IsValid(_altLaneMoveState))
	{
		_altLaneMoveState->ConditionalBeginDestroy();
		_altLaneMoveState = nullptr;
	}
}

// Called every frame
void ALaneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Timeline ticks
	//_swapLaneTimeline->TickTimeline(DeltaTime);
	//if (IsValid(_swapLaneTimeline))
	//{
	//	_swapLaneTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	//}
	
	UPlayerMoveState* moveState = _currentMoveState->updateState();

	if (IsValid(moveState))
	{
		_currentMoveState = moveState;
		UE_LOG(LogLaneCharacter, Error, TEXT("STATE CHANGED!!!!!!!"));
	}

	_currentMoveState->update(DeltaTime);
}

// Called to bind functionality to input
void ALaneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ALaneCharacter::startJump);
	PlayerInputComponent->BindAction(TEXT("MoveToLeftLane"), EInputEvent::IE_Pressed, this, &ALaneCharacter::switchToLeftLane);
	PlayerInputComponent->BindAction(TEXT("MoveToRightLane"), EInputEvent::IE_Pressed, this, &ALaneCharacter::switchToRightLane);
	PlayerInputComponent->BindAction(TEXT("TurnLeft"), EInputEvent::IE_Pressed, this, &ALaneCharacter::turnLeft);
	PlayerInputComponent->BindAction(TEXT("TurnRight"), EInputEvent::IE_Pressed, this, &ALaneCharacter::turnRight);
}

void ALaneCharacter::intersectionTransition()
{
	//_currentMoveState->transition();
}

void ALaneCharacter::startJump()
{
	Jump();
}

void ALaneCharacter::switchToLeftLane()
{
	_currentMoveState->switchToLeftLane();
}

void ALaneCharacter::switchToRightLane()
{
	_currentMoveState->switchToRightLane();
}

void ALaneCharacter::turnLeft()
{
	_currentMoveState->turnLeft();
}

void ALaneCharacter::turnRight()
{
	_currentMoveState->turnRight();
}

void ALaneCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	_currentMoveState->onBeginOverlap(OtherActor, OtherComp);
}

//void ALaneCharacter::swapLaneTimelineCallback(float position)
//{
//}
//
//void ALaneCharacter::swapLaneTimelineFinishedCallback()
//{
//}

