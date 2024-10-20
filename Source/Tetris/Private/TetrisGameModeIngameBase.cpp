// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisGameModeIngameBase.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#include "TetrisPlayManager.h"
#include "TetrisPlayerControllerIngameSingle.h"
#include "TetrisPlayerState.h"
#include "GoalSystemFactory.h"
#include "GoalSystemInterface.h"
#include "HUDSingle.h"
#include "Components/AudioComponent.h"

const FName ATetrisGameModeIngameBase::TetrisLevelName = FName(TEXT("TetrisLevel"));

ATetrisGameModeIngameBase::ATetrisGameModeIngameBase()
	: GoalSystemType(EGoalSystemType::None)
{
	// Unreal Editor에서 PlayerStateClass를 설정할 수 있도록 함
	PlayerStateClass = nullptr;
}

void ATetrisGameModeIngameBase::PostLogin(APlayerController* const NewPlayer)
{
	Super::PostLogin(NewPlayer);

	TetrisPlayerState = Cast<ATetrisPlayerState>(NewPlayer->PlayerState);
}

float ATetrisGameModeIngameBase::GetElapsedTime() const
{
	return UGameplayStatics::GetTimeSeconds(GetWorld()) - GameStartTime;
}

float ATetrisGameModeIngameBase::GetCurrentLevelNormalFallSpeed() const
{
	if (TetrisPlayerState)
	{
		return ATetrisGameModeIngameBase::CalculateNormalFallSpeed(TetrisPlayerState->GetGameLevel());
	}

	checkNoEntry();
	return 0.f;
}

void ATetrisGameModeIngameBase::UpdateGamePlay(const FTetrisGamePlayInfo& UpdateInfo)
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
		if (HUDWidget)
		{
			HUDWidget->UpdateDisplay(TetrisPlayerState->GetHUDSingleUpdateDisplayParams());
		}
	}
}

void ATetrisGameModeIngameBase::RunGameOver()
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

void ATetrisGameModeIngameBase::BeginPlay()
{
	Super::BeginPlay();

	StartGamePlay();
}

void ATetrisGameModeIngameBase::LevelUp()
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

void ATetrisGameModeIngameBase::Initialize()
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

void ATetrisGameModeIngameBase::SetInputMode()
{
	// Set the input mode to GameOnly
	const FInputModeGameOnly InputMode;
	InternalSetInputMode(InputMode);
}

void ATetrisGameModeIngameBase::InitializeEffect()
{
	Super::InitializeEffect();
}

void ATetrisGameModeIngameBase::StartGamePlay()
{
	GameStartTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	if (TetrisPlayManager)
	{
		TetrisPlayManager->EnterPhase(EPhase::Generation);
	}
}

float ATetrisGameModeIngameBase::CalculateNormalFallSpeed(const int32 GameLevel)
{
	const float A = 0.8f - ((GameLevel - 1) * 0.007f);
	const float B = static_cast<float>(GameLevel - 1);
	return FMath::Pow(A, B);
}
