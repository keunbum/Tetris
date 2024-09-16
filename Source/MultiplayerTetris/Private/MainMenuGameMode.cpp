// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

#include "TetrisSaveGameOption.h"

const FName AMainMenuGameMode::MainMenuLevelName(TEXT("MainMenuLevel"));
const FName AMainMenuGameMode::BGMCuePath(TEXT("/Game/Sound/BGM/Block_Dance_Cue"));

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMainMenuGameMode::Initialize()
{
	Super::Initialize();
}

void AMainMenuGameMode::SetInputMode()
{
	// Set the input mode to UIOnly
	const FInputModeUIOnly InputMode;
	InternalSetInputMode(InputMode);
}

void AMainMenuGameMode::LoadSetting()
{
	TetrisSaveGame = UTetrisSaveGameOption::LoadTetrisSaveGame();
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
