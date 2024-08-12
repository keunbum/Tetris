// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MenuBase.h"

#include "MenuButton.h"

void UMenuBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetInitialFocus();

	DefaultFocusedButtonIndex = 0;
}

FReply UMenuBase::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsNoButtonFocused())
	{
		SetDefaultMenuButtonFocus();
		return FReply::Handled();
	}

	const FKey Key = InKeyEvent.GetKey();
	const EMenuMoveDirection MenuMoveDirection = UMenuBase::GetMenuMoveDirection(Key);
	if (UMenuBase::IsMenuMoveDirectionValid(MenuMoveDirection))
	{
		const int32 MoveDelta = UMenuBase::GetMenuMoveDelta(MenuMoveDirection);
		MoveMenuButtonFocus(MoveDelta);
		return FReply::Handled();
	}

	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UMenuBase::SetInitialFocus()
{
	// 초기에 포커싱된 버튼 없음.
	FocusedButtonIndex = UMenuBase::InvalidButtonIndex;
	// 위젯 자체에는 포커싱이 걸려야 키보드 입력을 받을 수 있음.
	bIsFocusable = true;
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

void UMenuBase::SetDefaultMenuButtonFocus()
{
	UpdateMenuButtonFocus(DefaultFocusedButtonIndex);
}

EMenuMoveDirection UMenuBase::GetMenuMoveDirection(const FKey& Key)
{
	if (UMenuBase::IsUpKey(Key))
	{
		return EMenuMoveDirection::Up;
	}
	if (UMenuBase::IsDownKey(Key))
	{
		return EMenuMoveDirection::Down;
	}
	if (UMenuBase::IsLeftKey(Key))
	{
		return EMenuMoveDirection::Left;
	}
	if (UMenuBase::IsRightKey(Key))
	{
		return EMenuMoveDirection::Right;
	}

	return EMenuMoveDirection::None;
}

void UMenuBase::UpdateMenuButtonFocus(const int32 NewFocusedButtonIndex)
{
	FocusedButtonIndex = NewFocusedButtonIndex;
	MenuButtons[FocusedButtonIndex]->SetFocus();
}

void UMenuBase::MoveMenuButtonFocus(const int32 MoveDelta)
{
	const int32 NewFocusedButtonIndex = (FocusedButtonIndex + MoveDelta + MenuButtons.Num()) % MenuButtons.Num();
	UpdateMenuButtonFocus(NewFocusedButtonIndex);
}
