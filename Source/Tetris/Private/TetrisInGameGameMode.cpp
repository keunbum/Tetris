// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisIngameGameMode.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#include "TetrisPlayManager.h"
#include "TetrisPlayerControllerSingle.h"
#include "TetrisPlayerStateBase.h"
#include "GoalSystemFactory.h"
#include "GoalSystemInterface.h"
#include "HUDSingle.h"

const FName ATetrisIngameGameMode::TetrisLevelName = FName(TEXT("TetrisLevel"));

ATetrisIngameGameMode::ATetrisIngameGameMode()
	: GoalSystemType(EGoalSystemType::None)
{
	// Unreal Editor에서 PlayerStateClass를 설정할 수 있도록 함
	PlayerStateClass = nullptr;
}

void ATetrisIngameGameMode::PostLogin(APlayerController* const NewPlayer)
{
	Super::PostLogin(NewPlayer);

	TetrisPlayerState = Cast<ATetrisPlayerStateBase>(NewPlayer->PlayerState);
}

float ATetrisIngameGameMode::GetElapsedTime() const
{
	return UGameplayStatics::GetTimeSeconds(GetWorld()) - GameStartTime;
}

float ATetrisIngameGameMode::GetCurrentLevelNormalFallSpeed() const
{
	if (TetrisPlayerState)
	{
		return ATetrisIngameGameMode::CalculateNormalFallSpeed(TetrisPlayerState->GetGameLevel());
	}

	return 0.f;
}

void ATetrisIngameGameMode::UpdateGamePlay(const FTetrisGamePlayInfo& UpdateInfo)
{
	if (TetrisPlayerState)
	{
		TetrisPlayerState->UpdateState(UpdateInfo);

		if (GoalSystem)
		{
			const bool bIsLevelUpCondition = GoalSystem->IsLevelUpCondition(*TetrisPlayerState);
			if (bIsLevelUpCondition)
			{
				LevelUp();
			}
		}

		// Update HUD
		if (HUDWidget)
		{
			HUDWidget->UpdateDisplay(TetrisPlayerState->GetHUDSingleUpdateDisplayParams());
		}
	}
}

void ATetrisIngameGameMode::RunGameOver()
{
	// 추후에 추가 로직 작성할 가능성 있음
	if (TetrisPlayerController)
	{
		TetrisPlayerController->SetInputModeUIOnlyAndGameOver();
	}
}

void ATetrisIngameGameMode::BeginPlay()
{
	Super::BeginPlay();

	StartGamePlay();
}

void ATetrisIngameGameMode::LevelUp()
{
	if (TetrisPlayerState && TetrisPlayManager)
	{
		TetrisPlayerState->LevelUp(GoalSystem.GetInterface());

		const float OldNormalFallSpeed = TetrisPlayManager->GetNormalFallSpeed();
		const float NewNormalFallSpeed = GetCurrentLevelNormalFallSpeed();
		check(OldNormalFallSpeed != NewNormalFallSpeed); // If this is not true, the level up system is not working properly.
		TetrisPlayManager->SetNormalFallSpeed(NewNormalFallSpeed);
		UE_LOG(LogTemp, Warning, TEXT("Level Up! New NormalFallSpeed: %f"), NewNormalFallSpeed);
	}
}

void ATetrisIngameGameMode::Initialize()
{
	Super::Initialize();

	if (UWorld* const World = GetWorld())
	{

		// TetrisPlayManager
		TetrisPlayManager = World->SpawnActor<ATetrisPlayManager>(TetrisPlayManagerClass);

		// TetrisPlayerController
		TetrisPlayerController = Cast<ATetrisPlayerControllerSingle>(UGameplayStatics::GetPlayerController(World, PlayerIndex));

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
		HUDWidget = CreateWidget<UHUDSingle>(World, HUDWidgetClass);

		/** Call Initialize methods */
		if (TetrisPlayManager)
		{
			TetrisPlayManager->Initialize();
		}
		if (TetrisPlayerController)
		{
			TetrisPlayerController->Initialize();
		}
		if (TetrisPlayerState)
		{
			TetrisPlayerState->Initialize(GoalSystem.GetInterface());

			if (HUDWidget)
			{
				HUDWidget->InitializeHUD(TetrisPlayerState->GetHUDSingleUpdateDisplayParams(), this);
			}
		}
	}
}

void ATetrisIngameGameMode::SetInputMode()
{
	// Set the input mode to GameOnly
	const FInputModeGameOnly InputMode;
	InternalSetInputMode(InputMode);
}

void ATetrisIngameGameMode::InitializeEffect()
{
	Super::InitializeEffect();
}

void ATetrisIngameGameMode::StartGamePlay()
{
	GameStartTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	if (TetrisPlayManager)
	{
		TetrisPlayManager->EnterPhase(EPhase::Generation);
	}
}

float ATetrisIngameGameMode::CalculateNormalFallSpeed(const int32 GameLevel)
{
	const float A = 0.8f - ((GameLevel - 1) * 0.007f);
	const float B = static_cast<float>(GameLevel - 1);
	return FMath::Pow(A, B);
}
