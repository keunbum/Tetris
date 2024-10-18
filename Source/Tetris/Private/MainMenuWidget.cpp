// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"

#include "TetrisGameModeIngameBase.h"
#include "TetrisWidgetPopupOption.h"
#include "MenuButton.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartButton)
	{
		MenuButtons.Add(StartButton);
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartClicked);
	}
	if (OptionButton)
	{
		MenuButtons.Add(OptionButton);
		OptionButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionClicked);
	}
	if (ExitButton)
	{
		MenuButtons.Add(ExitButton);
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitClicked);
	}

	if (WidgetPopupOptionClass)
	{
		WidgetPopupOption = CreateWidget<UTetrisWidgetPopupOption>(GetWorld(), WidgetPopupOptionClass);
	}
}

void UMainMenuWidget::OnStartClicked()
{
	// Open Tetris Level
	UGameplayStatics::OpenLevel(GetWorld(), ATetrisGameModeIngameBase::TetrisLevelName);
}

void UMainMenuWidget::OnOptionClicked()
{
	if (WidgetPopupOption)
	{
		WidgetPopupOption->AddToViewport();
	}
}

void UMainMenuWidget::OnExitClicked()
{
	// Exit Game
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
