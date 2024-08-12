// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MenuBase.h"

void UMenuBase::InitializeMenuButtons(const TArray<UMenuButton*>& InMenuButtons)
{
	MenuButtons = InMenuButtons;
	for (const UMenuButton* MenuButton : MenuButtons)
	{
		check(MenuButton != nullptr);
	}
}
