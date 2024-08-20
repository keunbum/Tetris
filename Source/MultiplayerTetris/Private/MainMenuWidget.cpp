// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"

#include "TetrisGameModeBase.h"
#include "MenuButton.h"

void UMainMenuWidget::SetInitialFocus()
{
	SetWidgetFocusOnly();
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

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetMenuButtons({ StartGameButton, ExitGameButton });

	StartGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameClicked);
	ExitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitGameClicked);
}
