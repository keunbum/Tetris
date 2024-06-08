// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "TetrisGameModeBase.h"

#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "Tetrimino.h"
#include "TetrisPlayManager.h"
#include "TetrisMatrix.h"
#include "Board.h"

ATetrisGameModeBase::ATetrisGameModeBase()
	: CurrentLevel(DefaultGameLevel)
{
}

void ATetrisGameModeBase::LevelUp()
{
	CurrentLevel += 1;

	const float NewFallSpeed = GetFallSpeed();
	TetrisPlayManager->SetNormalFallSpeed(NewFallSpeed);
}

void ATetrisGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
	StartGenerationPhase();
}

void ATetrisGameModeBase::Initialize()
{
	if (UWorld* const World = GetWorld())
	{
		TetrisPlayManager = World->SpawnActor<ATetrisPlayManager>();
		check(TetrisPlayManager != nullptr);
	}
}

void ATetrisGameModeBase::StartGenerationPhase()
{
	TetrisPlayManager->StartGenerationPhase();
}

void ATetrisGameModeBase::StartCompletionPhase()
{
}

float ATetrisGameModeBase::CalculateFallSpeed(const int32 Level)
{
	const float A = 0.8f - ((Level - 1) * 0.007f);
	const float B = static_cast<float>(Level - 1);
	return FMath::Pow(A, B);
}

