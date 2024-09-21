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
	}
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MenuButtons.IsEmpty())
	{
		SetMenuButtons({ StartButton, OptionButton, ExitButton });
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
	if (!OptionPopUpWidget)
	{
		check(OptionPopUpWidgetClass != nullptr);
		OptionPopUpWidget = CreateWidget<UOptionPopUpWidget>(GetWorld(), OptionPopUpWidgetClass);
		check(OptionPopUpWidget != nullptr);
	}

	OptionPopUpWidget->AddToViewport();
}

void UMainMenuWidget::OnExitClicked()
{
	// Exit Game
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
