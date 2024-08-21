// Copyright Ryu KeunBeom. All Rights Reserved.

#include "PauseMenuWidget.h"

#include "Kismet/GameplayStatics.h"

#include "MenuButton.h"
#include "TetrisPlayerControllerSingle.h"
#include "MainMenuGameMode.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetMenuButtons({ ResumeButton, RestartButton, ExitButton });

	SetDefaultMenuButtonFocus();

	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
	RestartButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnRestartClicked);
	ExitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnExitClicked);
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
