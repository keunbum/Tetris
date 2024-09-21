// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"

#include "TetrisInGameGameMode.h"
#include "OptionPopUpWidget.h"
#include "MenuButton.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MenuButtons.IsEmpty())
	{
		SetMenuButtons({ StartButton, ExitButton });
	}

	if (!StartButton->OnClicked.IsBound())
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartClicked);
	}
	if (!OptionButton->OnClicked.IsBound())
	{
		OptionButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionClicked);
	}
	if (!ExitButton->OnClicked.IsBound())
	{
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitClicked);
	}
}

void UMainMenuWidget::OnStartClicked()
{
	// Open Tetris Level
	UGameplayStatics::OpenLevel(GetWorld(), ATetrisInGameGameMode::TetrisLevelName);
}

void UMainMenuWidget::OnOptionClicked()
{
	// Open Option PopUp Widget
}

void UMainMenuWidget::OnExitClicked()
{
	// Exit Game
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
