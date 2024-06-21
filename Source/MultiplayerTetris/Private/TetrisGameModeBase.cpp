// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisGameModeBase.h"

#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "Tetrimino.h"
#include "TetrisPlayManager.h"
#include "Board.h"
#include "TetrisPlayerController.h"

ATetrisGameModeBase::ATetrisGameModeBase()
	: CurrentLevel(DefaultGameLevel)
	, TetrisPlayManagerClass(nullptr)
{
}

void ATetrisGameModeBase::LevelUp()
{
	CurrentLevel += 1;

	const float NewFallSpeed = GetNormalFallSpeed();
	TetrisPlayManager->SetNormalFallSpeed(NewFallSpeed);
}

void ATetrisGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
	StartGamePlay();
}

void ATetrisGameModeBase::Initialize()
{
	UWorld* const World = GetWorld();
	check(World != nullptr);

	check(TetrisPlayManagerClass != nullptr);
	TetrisPlayManager = World->SpawnActor<ATetrisPlayManager>(TetrisPlayManagerClass);
	check(TetrisPlayManager != nullptr);

	TetrisPlayerController = Cast<ATetrisPlayerController>(UGameplayStatics::GetPlayerController(World, PlayerIndex));
	check(TetrisPlayerController != nullptr);
}

void ATetrisGameModeBase::StartGamePlay()
{
	TetrisPlayManager->StartGenerationPhase();
}

float ATetrisGameModeBase::CalculateNormalFallSpeed(const int32 Level)
{
	const float A = 0.8f - ((Level - 1) * 0.007f);
	const float B = static_cast<float>(Level - 1);
	return FMath::Pow(A, B);
}

