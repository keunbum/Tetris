// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenu.h"

#include "MenuButton.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	check(StartGameButton != nullptr);
	check(ExitGameButton != nullptr);

	MenuButtons = { StartGameButton, ExitGameButton };

	UMenuButton* const InitialFocusedButton = MenuButtons[0];
	InitialFocusedButton->SetKeyboardFocus();
}

