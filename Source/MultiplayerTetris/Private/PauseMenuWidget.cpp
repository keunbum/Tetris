// Copyright Ryu KeunBeom. All Rights Reserved.

#include "PauseMenuWidget.h"

#include "Kismet/GameplayStatics.h"

#include "MenuButton.h"
#include "TetrisPlayerControllerSingle.h"
#include "MainMenuGameMode.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MenuButtons.IsEmpty())
	{
		SetMenuButtons({ ResumeButton, RestartButton, ExitButton });
	}

	SetDefaultMenuButtonFocus();

	if (!ResumeButton->OnClicked.IsBound())
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
	}
	if (!RestartButton->OnClicked.IsBound())
	{
		RestartButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnRestartClicked);
	}
	if (!ExitButton->OnClicked.IsBound())
	{
		ExitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnExitClicked);
	}
}

void UPauseMenuWidget::OnResumeClicked()
{
	if (ATetrisPlayerControllerSingle* const PlayerController = Cast<ATetrisPlayerControllerSingle>(GetOwningPlayer()))
	{
		PlayerController->SetInputModeGameOnlyAndUnPause();
	}
}

void UPauseMenuWidget::OnRestartClicked()
{
	// Restart the level
	if (UWorld* const World = GetWorld())
	{
		const FName CurrentLevelName = World->GetFName();
		UGameplayStatics::OpenLevel(this, CurrentLevelName);
	}
}

void UPauseMenuWidget::OnExitClicked()
{
	// Exit to Main Menu Level
	UGameplayStatics::OpenLevel(this, AMainMenuGameMode::MainMenuLevelName);
}
