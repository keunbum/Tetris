// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisWidgetMenuGameover.h"
#include "Kismet/GameplayStatics.h"

#include "MenuButton.h"
#include "TetrisGameModeMainMenu.h"

void UTetrisWidgetMenuGameover::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (RestartButton)
	{
		MenuButtons.Add(RestartButton);
		RestartButton->OnClicked.AddDynamic(this, &UTetrisWidgetMenuGameover::OnRestartClicked);
	}
	if (HomeButton)
	{
		MenuButtons.Add(HomeButton);
		HomeButton->OnClicked.AddDynamic(this, &UTetrisWidgetMenuGameover::OnHomeClicked);
	}
}

void UTetrisWidgetMenuGameover::OnRestartClicked()
{
	// Restart the level
	UGameplayStatics::OpenLevel(this, GetWorld()->GetFName());
}

void UTetrisWidgetMenuGameover::OnHomeClicked()
{
	// Go to the Main Menu
	UGameplayStatics::OpenLevel(this, ATetrisGameModeMainMenu::MainMenuLevelName);
}
