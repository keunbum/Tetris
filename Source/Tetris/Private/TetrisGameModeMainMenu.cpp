// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisGameModeMainMenu.h"
#include "TetrisWidgetMenuMain.h"

const FName ATetrisGameModeMainMenu::MainMenuLevelName(TEXT("MainMenuLevel"));

void ATetrisGameModeMainMenu::BeginPlay()
{
	Super::BeginPlay();

	// Create Main Menu Widget
	MainMenuWidget = CreateWidget<UTetrisWidgetMenuMain>(GetWorld(), MainMenuWidgetClass);
	if (MainMenuWidget)
	{
		MainMenuWidget->AddToViewport();
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
