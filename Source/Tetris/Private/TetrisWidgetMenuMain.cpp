// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisWidgetMenuMain.h"

#include "Kismet/GameplayStatics.h"

#include "TetrisGameModeIngameBase.h"
#include "TetrisWidgetPopupOption.h"
#include "MenuButton.h"

void UTetrisWidgetMenuMain::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartButton)
	{
		MenuButtons.Add(StartButton);
		StartButton->OnClicked.AddDynamic(this, &UTetrisWidgetMenuMain::OnStartClicked);
	}
	if (OptionButton)
	{
		MenuButtons.Add(OptionButton);
		OptionButton->OnClicked.AddDynamic(this, &UTetrisWidgetMenuMain::OnOptionClicked);
	}
	if (ExitButton)
	{
		MenuButtons.Add(ExitButton);
		ExitButton->OnClicked.AddDynamic(this, &UTetrisWidgetMenuMain::OnExitClicked);
	}

	if (WidgetPopupOptionClass)
	{
		WidgetPopupOption = CreateWidget<UTetrisWidgetPopupOption>(GetWorld(), WidgetPopupOptionClass);
	}
}

void UTetrisWidgetMenuMain::OnStartClicked()
{
	// Open Tetris Level
	UGameplayStatics::OpenLevel(GetWorld(), ATetrisGameModeIngameBase::TetrisLevelName);
}

void UTetrisWidgetMenuMain::OnOptionClicked()
{
	if (WidgetPopupOption)
	{
		WidgetPopupOption->AddToViewport();
	}
}

void UTetrisWidgetMenuMain::OnExitClicked()
{
	// Exit Game
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
