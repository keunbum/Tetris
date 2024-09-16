// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuGameMode.h"

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
	Super::LoadSetting();
}

bool AMainMenuGameMode::LoadSaveGameInstance()
{
	TetrisSaveGameOption = UTetrisSaveGameOption::LoadTetrisSaveGameOptionInstance();
	return TetrisSaveGameOption != nullptr;
}

void AMainMenuGameMode::LoadSoundSetting()
{
	BGMComponent = ATetrisGameModeBase::CreateAudioComponent(BGMCuePath);
}
