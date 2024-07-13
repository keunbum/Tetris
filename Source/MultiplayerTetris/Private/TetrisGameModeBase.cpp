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
#include "GoalSystemFactory.h"
#include "GoalSystemInterface.h"


ATetrisGameModeBase::ATetrisGameModeBase()
	: TetrisPlayManagerClass(nullptr)
{
	PlayerStateClass = ATetrisPlayerStateBase::StaticClass();
}

float ATetrisGameModeBase::GetCurrentLevelNormalFallSpeed() const
{
	return ATetrisGameModeBase::CalculateNormalFallSpeed(TetrisPlayerState->GetGameLevel());
}

void ATetrisGameModeBase::UpdateGamePlay(const FTetrisGamePlayInfo& UpdateInfo)
{
	TetrisPlayerState->UpdateState(UpdateInfo);

	check(GoalSystem);
	const bool bIsLevelUpCondition = GoalSystem->IsLevelUpCondition(*TetrisPlayerState);
	if (bIsLevelUpCondition)
	{
		TetrisPlayerState->LevelUp();

		const float OldNormalFallSpeed = TetrisPlayManager->GetNormalFallSpeed();
		const float NewNormalFallSpeed = GetCurrentLevelNormalFallSpeed();
		check(OldNormalFallSpeed != NewNormalFallSpeed); // If this is not true, the level up system is not working properly.
		TetrisPlayManager->SetNormalFallSpeed(NewNormalFallSpeed);
		const int32 NewGameLevel = TetrisPlayerState->GetGameLevel();
		UE_LOG(LogTemp, Warning, TEXT("Level Up! New Level: %d, New NormalFallSpeed: %f"), NewGameLevel, NewNormalFallSpeed);
	}
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

	/** Create GoalSystem */
	if (IGoalSystemInterface* const GoalSystemInterface = GoalSystemFactory::CreateGoalSystem(GoalSystemType, this))
	{
		if (UObject* const GoalSystemObject = Cast<UObject>(GoalSystemInterface))
		{
			GoalSystem.SetInterface(GoalSystemInterface);
			GoalSystem.SetObject(GoalSystemObject);
		}
	}

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
