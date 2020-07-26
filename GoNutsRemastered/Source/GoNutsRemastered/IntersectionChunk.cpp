// Fill out your copyright notice in the Description page of Project Settings.


#include "IntersectionChunk.h"

AIntersectionChunk::AIntersectionChunk()
    : ALevelChunk()
{
    _leftConnector = CreateDefaultSubobject<USceneComponent>(TEXT("LeftConnector"));
    _leftConnector->SetupAttachment(RootComponent);

    _rightConnector = CreateDefaultSubobject<USceneComponent>(TEXT("RightConnector"));
    _rightConnector->SetupAttachment(RootComponent);
}

USceneComponent* AIntersectionChunk::getLeftConnector() const
{
    return _leftConnector;
}

USceneComponent* AIntersectionChunk::getRightConnector() const
{
    return _rightConnector;
}

void AIntersectionChunk::BeginPlay()
{
    Super::BeginPlay();

    if (_chunkFeatures & !EChunkFeatures::Type::CF_PLAYER_TURN_LEFT)
    {
        _leftConnector->DestroyComponent();
    }

    if (_chunkFeatures & !EChunkFeatures::Type::CF_PLAYER_TURN_RIGHT)
    {
        _rightConnector->DestroyComponent();
    }
}
