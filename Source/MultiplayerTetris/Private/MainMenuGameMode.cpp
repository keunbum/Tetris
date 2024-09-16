// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

#include "TetrisPlayerControllerSingle.h"
#include "TetrisGameModeBase.h"
#include "TetrisSaveGame.h"

const FName AMainMenuGameMode::MainMenuLevelName(TEXT("MainMenuLevel"));
const FName AMainMenuGameMode::BGMCuePath(TEXT("/Game/Sound/BGM/Block_Dance_Cue"));

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
	TetrisSaveGame = UTetrisSaveGame::LoadTetrisSaveGame();
	if (!TetrisSaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuGameMode::LoadSoundSetting() - Failed to load TetrisSaveGame"));
		return;
	}

	LoadSoundSetting();
}

void AMainMenuGameMode::LoadSoundSetting()
{
	BGMComponent = CreateAudioComponent(*BGMCuePath.ToString());
}

UAudioComponent* AMainMenuGameMode::CreateAudioComponent(const TCHAR* CuePath) const
{
	if (USoundCue* const SoundCue = LoadObject<USoundCue>(nullptr, CuePath))
	{
		if (UAudioComponent* const AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), SoundCue))
		{
			return AudioComponent;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MainMenuGameMode::CreateAudioComponent() - Failed to spawn AudioComponent"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuGameMode::CreateAudioComponent() - Failed to load SoundCue"));
	}

	return nullptr;
}

void AMainMenuGameMode::SetAudioComponentVolume(UAudioComponent* const AudioComponent, const float Volume)
{
	if (AudioComponent)
	{
		AudioComponent->SetVolumeMultiplier(Volume);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuGameMode::SetAudioComponentVolume() - AudioComponent is nullptr"));
	}
}
