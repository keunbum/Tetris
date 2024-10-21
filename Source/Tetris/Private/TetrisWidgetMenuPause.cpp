// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisWidgetMenuPause.h"

#include "Kismet/GameplayStatics.h"

#include "MenuButton.h"
#include "TetrisPlayerControllerIngameSingle.h"
#include "TetrisGameModeMainMenu.h"
#include "TetrisWidgetPopupOption.h"

void UTetrisWidgetMenuPause::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ResumeButton)
	{
		MenuButtons.Add(ResumeButton);
		ResumeButton->OnClicked.AddDynamic(this, &UTetrisWidgetMenuPause::OnResumeClicked);
	}
	if (RestartButton)
	{
		MenuButtons.Add(RestartButton);
		RestartButton->OnClicked.AddDynamic(this, &UTetrisWidgetMenuPause::OnRestartClicked);
	}
	if (OptionButton)
	{
		MenuButtons.Add(OptionButton);
		OptionButton->OnClicked.AddDynamic(this, &UTetrisWidgetMenuPause::OnOptionClicked);
	}
	if (ExitButton)
	{
		MenuButtons.Add(ExitButton);
		ExitButton->OnClicked.AddDynamic(this, &UTetrisWidgetMenuPause::OnExitClicked);
	}

	if (WidgetPopupOptionClass)
	{
		WidgetPopupOption = CreateWidget<UTetrisWidgetPopupOption>(GetWorld(), WidgetPopupOptionClass);
	}

	TetrisPlayerController = Cast<ATetrisPlayerControllerIngameSingle>(GetOwningPlayer());
}

void UTetrisWidgetMenuPause::NativeConstruct()
{
	Super::NativeConstruct();

	SetDefaultMenuButtonFocus();
}

FReply UTetrisWidgetMenuPause::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

	if (static const TArray<FKey> ResumeKeys = { EKeys::Escape, EKeys::Q };
		ResumeKeys.Contains(Key))
	{
		OnResumeClicked();
		return FReply::Handled();
	}

	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UTetrisWidgetMenuPause::OnResumeClicked()
{
	if (TetrisPlayerController)
	{
		TetrisPlayerController->SetInputModeGameOnlyAndUnPause();
	}
}

void UTetrisWidgetMenuPause::OnRestartClicked()
{
	// Restart the level
	UGameplayStatics::OpenLevel(this, GetWorld()->GetFName());
}

void UTetrisWidgetMenuPause::OnOptionClicked()
{
	if (WidgetPopupOption)
	{
		WidgetPopupOption->AddToViewport();
	}
}

void UTetrisWidgetMenuPause::OnExitClicked()
{
	// Exit to Main Menu Level
	UGameplayStatics::OpenLevel(this, ATetrisGameModeMainMenu::MainMenuLevelName);
}
