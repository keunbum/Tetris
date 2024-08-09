// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenu.h"

#include "MenuButton.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	MenuButtons = { StartGameButton, ExitGameButton };
	for (UMenuButton* const MenuButton : MenuButtons)
	{
		check(MenuButton != nullptr);
	}

	UMenuButton* const InitialFocusedButton = MenuButtons[0];
	InitialFocusedButton->SetKeyboardFocus();
}

