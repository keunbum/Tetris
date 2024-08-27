// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"

#include "TetrisGameModeBase.h"
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
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameClicked);
	}
	if (!ExitButton->OnClicked.IsBound())
	{
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitGameClicked);
	}
}

void UMainMenuWidget::OnStartGameClicked()
{
	// Open Tetris Level
	UGameplayStatics::OpenLevel(GetWorld(), ATetrisGameModeBase::TetrisLevelName);
}

void UMainMenuWidget::OnExitGameClicked()
{
	// Exit Game
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
