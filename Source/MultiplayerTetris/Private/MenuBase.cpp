// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MenuBase.h"

#include "MenuButton.h"

FReply UMenuBase::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsNoButtonFocused())
	{
		SetInitialMenuButtonFocus();
		return FReply::Handled();
	}

	const FKey Key = InKeyEvent.GetKey();
	const EMenuMoveDirection MenuMoveDirection = UMenuBase::GetMenuMoveDirection(Key);
	if (IsMenuMoveDirectionValid(MenuMoveDirection))
	{
		const int32 MoveDelta = GetMenuMoveDelta(MenuMoveDirection);
		MoveMenuButtonFocus(MoveDelta);
		return FReply::Handled();
	}

	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UMenuBase::SetInitialFocus()
{
	// 초기에 포커싱된 버튼 없음.
	FocusedButtonIndex = UMenuBase::InvalidButtonIndex;
	// 이 위젯 자체는 포커싱되어야 함.
	bIsFocusable = true;
	// 위젯 자체에는 포커싱이 걸려야 키보드 입력을 받을 수 있음.
	SetFocus();
}

void UMenuBase::InitializeMenuButtons(const TArray<UMenuButton*>& InMenuButtons)
{
	MenuButtons = InMenuButtons;
	for (const UMenuButton* MenuButton : MenuButtons)
	{
		check(MenuButton != nullptr);
	}
}

void UMenuBase::SetInitialMenuButtonFocus()
{
	SetMenuButtonFocus(InitialFocusedButtonIndex);
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
