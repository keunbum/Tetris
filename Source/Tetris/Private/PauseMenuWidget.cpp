// Copyright Ryu KeunBeom. All Rights Reserved.

#include "PauseMenuWidget.h"

#include "Kismet/GameplayStatics.h"

#include "MenuButton.h"
#include "TetrisPlayerControllerIngameSingle.h"
#include "TetrisGameModeMainMenu.h"
#include "TetrisWidgetPopupOption.h"

void UPauseMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ResumeButton)
	{
		MenuButtons.Add(ResumeButton);
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
	}
	if (RestartButton)
	{
		MenuButtons.Add(RestartButton);
		RestartButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnRestartClicked);
	}
	if (OptionButton)
	{
		MenuButtons.Add(OptionButton);
		OptionButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnOptionClicked);
	}
	if (ExitButton)
	{
		MenuButtons.Add(ExitButton);
		ExitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnExitClicked);
	}

	if (WidgetPopupOptionClass)
	{
		WidgetPopupOption = CreateWidget<UTetrisWidgetPopupOption>(GetWorld(), WidgetPopupOptionClass);
	}

	TetrisPlayerController = Cast<ATetrisPlayerControllerIngameSingle>(GetOwningPlayer());
}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetDefaultMenuButtonFocus();
}

FReply UPauseMenuWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

	if (static const TArray<FKey> ResumeKeys = { EKeys::Q, EKeys::BackSpace };
		ResumeKeys.Contains(Key))
	{
		OnResumeClicked();
		return FReply::Handled();
	}

	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UPauseMenuWidget::OnResumeClicked()
{
	if (TetrisPlayerController)
	{
		TetrisPlayerController->SetInputModeGameOnlyAndUnPause();
	}
}

void UPauseMenuWidget::OnRestartClicked()
{
	// Restart the level
	UGameplayStatics::OpenLevel(this, GetWorld()->GetFName());
}

void UPauseMenuWidget::OnOptionClicked()
{
	if (WidgetPopupOption)
	{
		WidgetPopupOption->AddToViewport();
	}
}

void UPauseMenuWidget::OnExitClicked()
{
	// Exit to Main Menu Level
	UGameplayStatics::OpenLevel(this, ATetrisGameModeMainMenu::MainMenuLevelName);
}
