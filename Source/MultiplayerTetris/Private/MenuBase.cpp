// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MenuBase.h"

#include "MenuButton.h"

void UMenuBase::InitializeMenuButtons(const TArray<UMenuButton*>& InMenuButtons)
{
	MenuButtons = InMenuButtons;
	for (const UMenuButton* MenuButton : MenuButtons)
	{
		check(MenuButton != nullptr);
	}
}

EMenuMoveDirection UMenuBase::GetMenuMoveDirection(const FKey& Key)
{
	if (Key == EKeys::Up || Key == EKeys::Gamepad_DPad_Up || Key == EKeys::Gamepad_LeftStick_Up)
	{
		return EMenuMoveDirection::Up;
	}
	if (Key == EKeys::Down || Key == EKeys::Gamepad_DPad_Down || Key == EKeys::Gamepad_LeftStick_Down)
	{
		return EMenuMoveDirection::Down;
	}
	if (Key == EKeys::Left || Key == EKeys::Gamepad_DPad_Left || Key == EKeys::Gamepad_LeftStick_Left)
	{
		return EMenuMoveDirection::Left;
	}
	if (Key == EKeys::Right || Key == EKeys::Gamepad_DPad_Right || Key == EKeys::Gamepad_LeftStick_Right)
	{
		return EMenuMoveDirection::Right;
	}

	return EMenuMoveDirection::None;
}

void UMenuBase::SetMenuButtonFocus(const int32 NewFocusedButtonIndex)
{
	FocusedButtonIndex = NewFocusedButtonIndex;
	MenuButtons[FocusedButtonIndex]->SetFocus();
}

void UMenuBase::MoveMenuButtonFocus(const int32 MoveDelta)
{
	const int32 NewFocusedButtonIndex = (FocusedButtonIndex + MoveDelta + MenuButtons.Num()) % MenuButtons.Num();
	SetMenuButtonFocus(NewFocusedButtonIndex);
}
