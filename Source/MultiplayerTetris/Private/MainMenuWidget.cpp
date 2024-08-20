// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"

#include "TetrisGameModeBase.h"
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

	InitializeMenuButtons({ StartGameButton, ExitGameButton });
}
