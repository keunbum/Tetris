// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisGameModeBase.h"

#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "TetriminoBase.h"
#include "TetrisPlayManager.h"
#include "Board.h"
#include "TetrisPlayerController.h"
#include "TetrisPlayerStateBase.h"

ATetrisGameModeBase::ATetrisGameModeBase()
	: CurrentLevel(DefaultGameLevel)
	, TetrisPlayManagerClass(nullptr)
{
	PlayerStateClass = ATetrisPlayerStateBase::StaticClass();
}

void ATetrisGameModeBase::PostLogin(APlayerController* const NewPlayer)
{
	Super::PostLogin(NewPlayer);

	TetrisPlayerState = Cast<ATetrisPlayerStateBase>(NewPlayer->PlayerState);
	check(TetrisPlayerState != nullptr);
	// TODO: Initialize TetrisPlayerState (필요하다면)
}

void ATetrisGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
	StartGamePlay();
}

void ATetrisGameModeBase::LevelUp()
{
	// TODO: Implement LevelUp
}

void ATetrisGameModeBase::Initialize()
{
	/** Create */
	UWorld* const World = GetWorld();
	check(World != nullptr);

	check(TetrisPlayManagerClass != nullptr);
	TetrisPlayManager = World->SpawnActor<ATetrisPlayManager>(TetrisPlayManagerClass);
	check(TetrisPlayManager != nullptr);

	TetrisPlayerController = Cast<ATetrisPlayerController>(UGameplayStatics::GetPlayerController(World, PlayerIndex));
	check(TetrisPlayerController != nullptr);

	/** Call Initialize methods */
	TetrisPlayManager->Initialize();
	TetrisPlayerController->Initialize();
}

void ATetrisGameModeBase::StartGamePlay()
{
	TetrisPlayManager->StartGenerationPhase();
}

float ATetrisGameModeBase::CalculateNormalFallSpeed(const int32 GameLevel)
{
	const float A = 0.8f - ((GameLevel - 1) * 0.007f);
	const float B = static_cast<float>(GameLevel - 1);
	return FMath::Pow(A, B);
}
