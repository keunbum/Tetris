// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisGameModeBase.h"

#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "TetriminoBase.h"
#include "TetrisPlayManager.h"
#include "Board.h"
#include "TetrisPlayerController.h"
#include "TetrisPlayerStateBase.h"
#include "GoalSystemFactory.h"
#include "GoalSystemInterface.h"
#include "HUDSingle.h"


ATetrisGameModeBase::ATetrisGameModeBase()
	: GoalSystemType(EGoalSystemType::None)
	, TetrisPlayManagerClass(nullptr)
{
	// Unreal Editor에서 PlayerStateClass를 설정할 수 있도록 함
	PlayerStateClass = nullptr;
}

float ATetrisGameModeBase::GetCurrentLevelNormalFallSpeed() const
{
	return ATetrisGameModeBase::CalculateNormalFallSpeed(TetrisPlayerState->GetGameLevel());
}

void ATetrisGameModeBase::UpdateGamePlay(const FTetrisGamePlayInfo& UpdateInfo)
{
	TetrisPlayerState->UpdateState(UpdateInfo);

	check(GoalSystem.GetObject() != nullptr);
	const bool bIsLevelUpCondition = GoalSystem->IsLevelUpCondition(*TetrisPlayerState);
	if (bIsLevelUpCondition)
	{
		LevelUp();
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

void ATetrisGameModeBase::LevelUp()
{
	const int32 LevelUpLineCountGoal = GoalSystem->GetLevelUpLineCountGoal(TetrisPlayerState->GetGameLevel());
	TetrisPlayerState->LevelUp(LevelUpLineCountGoal);

	const float OldNormalFallSpeed = TetrisPlayManager->GetNormalFallSpeed();
	const float NewNormalFallSpeed = GetCurrentLevelNormalFallSpeed();
	check(OldNormalFallSpeed != NewNormalFallSpeed); // If this is not true, the level up system is not working properly.
	TetrisPlayManager->SetNormalFallSpeed(NewNormalFallSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Level Up! New NormalFallSpeed: %f"), NewNormalFallSpeed);

	// Update HUD
	check(HUDWidget != nullptr);
	const int32 NewGameLevel = TetrisPlayerState->GetGameLevel();
	HUDWidget->UpdateLevelDisplay(NewGameLevel);
}

void ATetrisGameModeBase::Initialize()
{
	/** Create */
	UWorld* const World = GetWorld();
	check(World != nullptr);

	check(GoalSystemType != EGoalSystemType::None);

	// TetrisPlayManager
	check(TetrisPlayManagerClass != nullptr);
	TetrisPlayManager = World->SpawnActor<ATetrisPlayManager>(TetrisPlayManagerClass);
	check(TetrisPlayManager != nullptr);

	// TetrisPlayerController
	TetrisPlayerController = Cast<ATetrisPlayerController>(UGameplayStatics::GetPlayerController(World, PlayerIndex));
	check(TetrisPlayerController != nullptr);

	// GoalSystem
	if (IGoalSystemInterface* const GoalSystemInterface = GoalSystemFactory::CreateGoalSystemInterface(GoalSystemType, this))
	{
		if (UObject* const GoalSystemObject = Cast<UObject>(GoalSystemInterface))
		{
			GoalSystem.SetInterface(GoalSystemInterface);
			GoalSystem.SetObject(GoalSystemObject);
		}
	}

	// HUDWidget
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UHUDSingle>(World, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			HUDWidget->UpdateLevelDisplay(TetrisPlayerState->GetGameLevel());
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
