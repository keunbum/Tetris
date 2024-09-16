// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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
	TetrisSaveGameOption = UTetrisSaveGameOption::LoadTetrisSaveGame();
	if (!TetrisSaveGameOption)
	{
		return;
	}

	LoadSoundSetting();
}

void AMainMenuGameMode::LoadSoundSetting()
{
	BGMComponent = ATetrisGameModeBase::CreateAudioComponent(BGMCuePath);
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
