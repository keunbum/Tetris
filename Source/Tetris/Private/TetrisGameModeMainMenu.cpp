// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisGameModeMainMenu.h"
#include "MainMenuWidget.h"

const FName ATetrisGameModeMainMenu::MainMenuLevelName(TEXT("MainMenuLevel"));

void ATetrisGameModeMainMenu::BeginPlay()
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

void ATetrisGameModeMainMenu::Initialize()
{
	Super::Initialize();
}

void ATetrisGameModeMainMenu::SetInputMode()
{
	// Set the input mode to UIOnly
	const FInputModeUIOnly InputMode;
	InternalSetInputMode(InputMode);
}

void ATetrisGameModeMainMenu::InitializeEffect()
{
	Super::InitializeEffect();
}
