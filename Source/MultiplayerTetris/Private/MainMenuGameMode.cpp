// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuGameMode.h"

const FName AMainMenuGameMode::MainMenuLevelName(TEXT("MainMenuLevel"));
const FName AMainMenuGameMode::BGMCuePath(TEXT("/Game/Audio/BGM/Whispers_of_the_Sea_Cue"));

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

void AMainMenuGameMode::InitDefaultEffect()
{
	Super::InitDefaultEffect();

	// Init Default Audio
	BGMComponent = CreateAudioComponent(BGMCuePath);
	check(BGMComponent != nullptr);
}
