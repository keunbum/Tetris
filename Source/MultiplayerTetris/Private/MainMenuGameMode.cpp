// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

#include "TetrisPlayerControllerSingle.h"
#include "TetrisGameModeBase.h"
#include "TetrisSaveGame.h"

const FName AMainMenuGameMode::MainMenuLevelName(TEXT("MainMenuLevel"));

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void AMainMenuGameMode::ToggleBGM(const bool bIsBGMOn)
{
	if (BGMComponent)
	{
		if (bIsBGMOn)
		{
			BGMComponent->Play();
		}
		else
		{
			BGMComponent->Stop();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuGameMode::ToggleBGM() - BGMComponent is nullptr"));
	}
}

void AMainMenuGameMode::Initialize()
{
	SetInputMode();
	LoadSetting();
	
	//PlayEffect();
}

void AMainMenuGameMode::SetInputMode()
{
	if (ATetrisPlayerControllerSingle* const TetrisPlayerController = Cast<ATetrisPlayerControllerSingle>(UGameplayStatics::GetPlayerController(GetWorld(), ATetrisGameModeBase::PlayerIndex)))
	{
		// Set the input mode to UIOnly
		const FInputModeUIOnly InputMode;
		TetrisPlayerController->SetInputMode(InputMode);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuGameMode::BeginPlay() - Failed to get TetrisPlayerController"));
	}
}

void AMainMenuGameMode::LoadSetting()
{
	LoadSoundSetting();
}

void AMainMenuGameMode::LoadSoundSetting()
{
	TetrisSaveGame = Cast<UTetrisSaveGame>(UGameplayStatics::LoadGameFromSlot(UTetrisSaveGame::CommonOptionSaveSlotName.ToString(), UTetrisSaveGame::UserIndex));
	check(TetrisSaveGame != nullptr);
	if (ensureMsgf(TetrisSaveGame, TEXT("MainMenuGameMode::LoadBGMSetting() - Failed to load TetrisSaveGame")))
	{
		// Load BGM Setting
		if (TetrisSaveGame->bIsMainMenuBGMOn)
		{
			static ConstructorHelpers::FObjectFinder<USoundCue> BGMCue(TEXT("/Game/Sound/BGM/Block_Dance_Cue"));
			if (BGMCue.Succeeded())
			{
				BGMComponent = UGameplayStatics::SpawnSound2D(this, BGMCue.Object);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("MainMenuGameMode::LoadBGMSetting() - Failed to find Block_Dance_Cue"));
			}
		}
	}
}

void AMainMenuGameMode::PlayEffect()
{
	PlaySoundEffect();
}

void AMainMenuGameMode::PlaySoundEffect()
{
	if (TetrisSaveGame->bIsMainMenuBGMOn && BGMComponent)
	{
		BGMComponent->Play();
	}
}

void AMainMenuGameMode::SaveSetting(const FString& SlotName)
{
	UGameplayStatics::SaveGameToSlot(TetrisSaveGame, SlotName, UTetrisSaveGame::UserIndex);
}
