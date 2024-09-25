// Copyright Ryu KeunBeom. All Rights Reserved.

#include "GameOverMenuWidget.h"
#include "Kismet/GameplayStatics.h"

#include "MenuButton.h"
#include "TetrisPlayerControllerSingle.h"
#include "MainMenuGameMode.h"

void UGameOverMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MenuButtons.IsEmpty())
	{
		SetMenuButtons({ RestartButton, HomeButton });
	}

	if (!RestartButton->OnClicked.IsBound())
	{
		RestartButton->OnClicked.AddDynamic(this, &UGameOverMenuWidget::OnRestartClicked);
	}
	if (!HomeButton->OnClicked.IsBound())
	{
		HomeButton->OnClicked.AddDynamic(this, &UGameOverMenuWidget::OnHomeClicked);
	}
}

void UGameOverMenuWidget::OnRestartClicked()
{
	// Restart the level
	UGameplayStatics::OpenLevel(this, GetWorld()->GetFName());
}

void UGameOverMenuWidget::OnHomeClicked()
{
	// Go to the Main Menu
	UGameplayStatics::OpenLevel(this, AMainMenuGameMode::MainMenuLevelName);
}
