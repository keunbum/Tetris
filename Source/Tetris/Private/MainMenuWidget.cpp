// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"

#include "TetrisInGameGameMode.h"
#include "OptionPopUpWidget.h"
#include "MenuButton.h"

const FName UMainMenuWidget::OptionPopUpWidgetPath(TEXT("/Game/UI/WB_OptionPopUp"));

UMainMenuWidget::UMainMenuWidget()
{
	static ConstructorHelpers::FClassFinder<UOptionPopUpWidget> OptionPopUpBPClass(*OptionPopUpWidgetPath.ToString());
	if (ensureMsgf(OptionPopUpBPClass.Succeeded(), TEXT("Failed to find OptionPopUpWidget BP class.")))
	{
		OptionPopUpWidgetClass = OptionPopUpBPClass.Class;
		OptionPopUpWidget = CreateWidget<UOptionPopUpWidget>(GetWorld(), OptionPopUpWidgetClass);
	}
}

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
}

void UMainMenuWidget::OnStartClicked()
{
	// Open Tetris Level
	UGameplayStatics::OpenLevel(GetWorld(), ATetrisInGameGameMode::TetrisLevelName);
}

void UMainMenuWidget::OnOptionClicked()
{
	if (OptionPopUpWidget)
	{
		OptionPopUpWidget->AddToViewport();
	}
}

void UMainMenuWidget::OnExitClicked()
{
	// Exit Game
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
