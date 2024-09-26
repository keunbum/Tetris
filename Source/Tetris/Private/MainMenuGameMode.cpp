// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuGameMode.h"
#include "MainMenuWidget.h"

const FName AMainMenuGameMode::MainMenuLevelName(TEXT("MainMenuLevel"));
const FName AMainMenuGameMode::BGMCuePath(TEXT("/Game/Audio/BGM/Whispers_of_the_Sea_Cue"));

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Create Main Menu Widget
	if (!MainMenuWidget)
	{
		if (MainMenuWidgetClass)
		{
			MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
			if (MainMenuWidget)
			{
				MainMenuWidget->AddToViewport();
			}
		}
	}
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

void AMainMenuGameMode::InitializeDefaultEffect()
{
	Super::InitializeDefaultEffect();

	// Init Default Audio
	BGMComponent = CreateAudioComponent(BGMCuePath);
}