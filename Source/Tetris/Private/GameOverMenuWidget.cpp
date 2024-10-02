// Copyright Ryu KeunBeom. All Rights Reserved.

#include "GameoverMenuWidget.h"
#include "Kismet/GameplayStatics.h"

#include "MenuButton.h"
#include "TetrisPlayerControllerSingle.h"
#include "MainMenuGameMode.h"

void UGameoverMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (RestartButton)
	{
		MenuButtons.Add(RestartButton);
		RestartButton->OnClicked.AddDynamic(this, &UGameoverMenuWidget::OnRestartClicked);
	}
	if (HomeButton)
	{
		MenuButtons.Add(HomeButton);
		HomeButton->OnClicked.AddDynamic(this, &UGameoverMenuWidget::OnHomeClicked);
	}
}

void UGameoverMenuWidget::OnRestartClicked()
{
	// Restart the level
	UGameplayStatics::OpenLevel(this, GetWorld()->GetFName());
}

void UGameoverMenuWidget::OnHomeClicked()
{
	// Go to the Main Menu
	UGameplayStatics::OpenLevel(this, AMainMenuGameMode::MainMenuLevelName);
}
