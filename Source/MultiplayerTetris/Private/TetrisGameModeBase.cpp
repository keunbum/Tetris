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
{
	// Unreal Editor에서 PlayerStateClass를 설정할 수 있도록 함
	PlayerStateClass = nullptr;
}

void ATetrisGameModeBase::PostLogin(APlayerController* const NewPlayer)
{
	Super::PostLogin(NewPlayer);

	TetrisPlayerState = Cast<ATetrisPlayerStateBase>(NewPlayer->PlayerState);
	check(TetrisPlayerState != nullptr);
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

	// Update HUD
	HUDWidget->UpdateDisplay(TetrisPlayerState);
}

void ATetrisGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
	StartGamePlay();
}

void ATetrisGameModeBase::LevelUp()
{
	TetrisPlayerState->LevelUp(GoalSystem.GetInterface());

	const float OldNormalFallSpeed = TetrisPlayManager->GetNormalFallSpeed();
	const float NewNormalFallSpeed = GetCurrentLevelNormalFallSpeed();
	check(OldNormalFallSpeed != NewNormalFallSpeed); // If this is not true, the level up system is not working properly.
	TetrisPlayManager->SetNormalFallSpeed(NewNormalFallSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Level Up! New NormalFallSpeed: %f"), NewNormalFallSpeed);
}

void ATetrisGameModeBase::Initialize()
{
	/** Create */
	UWorld* const World = GetWorld();
	check(World != nullptr);

	// TetrisPlayManager
	check(TetrisPlayManagerClass != nullptr);
	TetrisPlayManager = World->SpawnActor<ATetrisPlayManager>(TetrisPlayManagerClass);
	check(TetrisPlayManager != nullptr);

	// TetrisPlayerController
	TetrisPlayerController = Cast<ATetrisPlayerController>(UGameplayStatics::GetPlayerController(World, PlayerIndex));
	check(TetrisPlayerController != nullptr);

	// GoalSystem
	check(GoalSystemType != EGoalSystemType::None);
	if (IGoalSystemInterface* const GoalSystemInterface = GoalSystemFactory::CreateGoalSystemInterface(GoalSystemType, this))
	{
		if (UObject* const GoalSystemObject = Cast<UObject>(GoalSystemInterface))
		{
			GoalSystem.SetInterface(GoalSystemInterface);
			GoalSystem.SetObject(GoalSystemObject);
		}
	}

	// HUDWidget
	check(HUDWidgetClass != nullptr);
	HUDWidget = CreateWidget<UHUDSingle>(World, HUDWidgetClass);
	check(HUDWidget != nullptr);

	/** Call Initialize methods */
	TetrisPlayManager->Initialize();
	TetrisPlayerController->Initialize();
	TetrisPlayerState->Initialize(GoalSystem.GetInterface());
	HUDWidget->InitializeHUD(TetrisPlayerState);
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
