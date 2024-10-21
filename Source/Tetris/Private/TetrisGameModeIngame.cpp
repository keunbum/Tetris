// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisGameModeIngame.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "TetrisPlayManager.h"
#include "TetrisPlayerControllerIngameSingle.h"
#include "TetrisPlayerState.h"
#include "GoalSystemFactory.h"
#include "GoalSystemInterface.h"
#include "Components/AudioComponent.h"
#include "TetrisHudIngame.h"

const FName ATetrisGameModeIngame::TetrisLevelName = FName(TEXT("TetrisLevel"));

ATetrisGameModeIngame::ATetrisGameModeIngame()
	: GoalSystemType(EGoalSystemType::None)
{
	PlayerStateClass = ATetrisPlayerState::StaticClass();
}

void ATetrisGameModeIngame::PostLogin(APlayerController* const NewPlayer)
{
	Super::PostLogin(NewPlayer);

	TetrisPlayerState = Cast<ATetrisPlayerState>(NewPlayer->PlayerState);
}

float ATetrisGameModeIngame::GetElapsedTime() const
{
	return UGameplayStatics::GetTimeSeconds(GetWorld()) - GameStartTime;
}

float ATetrisGameModeIngame::GetCurrentLevelNormalFallSpeed() const
{
	if (TetrisPlayerState)
	{
		return ATetrisGameModeIngame::CalculateNormalFallSpeed(TetrisPlayerState->GetGameLevel());
	}

	checkNoEntry();
	return 0.f;
}

void ATetrisGameModeIngame::UpdateGamePlay(const FTetrisGamePlayInfo& UpdateInfo)
{
	if (TetrisPlayerState)
	{
		TetrisPlayerState->UpdateState(UpdateInfo);

		const bool bIsLevelUpCondition = (GoalSystem && GoalSystem->IsLevelUpCondition(*TetrisPlayerState));
		if (bIsLevelUpCondition)
		{
			LevelUp();
		}

		// Update HUD
		if (TetrisPlayerController)
		{
			if (ATetrisHudIngame* const Hud = Cast<ATetrisHudIngame>(TetrisPlayerController->GetHUD()))
			{
				Hud->Update();
			}
		}
	}
}

void ATetrisGameModeIngame::RunGameOver()
{
	if (BgmComponent)
	{
		BgmComponent->FadeOut(BgmFadeOutTime, 0.f /* FadeVolumeLevel */);
	}

	if (TetrisPlayerController)
	{
		TetrisPlayerController->SetInputModeUIOnlyAndGameOver();
	}
}

void ATetrisGameModeIngame::BeginPlay()
{
	Super::BeginPlay();

	StartGamePlay();
}

void ATetrisGameModeIngame::LevelUp()
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

void ATetrisGameModeIngame::Initialize()
{
	Super::Initialize();

	if (UWorld* const World = GetWorld())
	{

		// TetrisPlayManager
		TetrisPlayManager = World->SpawnActor<ATetrisPlayManager>(TetrisPlayManagerClass);

		// TetrisPlayerController
		TetrisPlayerController = Cast<ATetrisPlayerControllerIngameSingle>(UGameplayStatics::GetPlayerController(World, PlayerIndex));

		// GoalSystem
		if (IGoalSystemInterface* const GoalSystemInterface = GoalSystemFactory::CreateGoalSystemInterface(GoalSystemType, this))
		{
			if (UObject* const GoalSystemObject = Cast<UObject>(GoalSystemInterface))
			{
				GoalSystem.SetInterface(GoalSystemInterface);
				GoalSystem.SetObject(GoalSystemObject);
			}
		}

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
		}
	}
}

void ATetrisGameModeIngame::SetInputMode()
{
	// Set the input mode to GameOnly
	const FInputModeGameOnly InputMode;
	InternalSetInputMode(InputMode);
}

void ATetrisGameModeIngame::InitializeEffect()
{
	Super::InitializeEffect();
}

void ATetrisGameModeIngame::StartGamePlay()
{
	GameStartTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	if (TetrisPlayManager)
	{
		TetrisPlayManager->EnterPhase(EPhase::Generation);
	}
}

float ATetrisGameModeIngame::CalculateNormalFallSpeed(const int32 GameLevel)
{
	const float A = 0.8f - ((GameLevel - 1) * 0.007f);
	const float B = static_cast<float>(GameLevel - 1);
	return FMath::Pow(A, B);
}
