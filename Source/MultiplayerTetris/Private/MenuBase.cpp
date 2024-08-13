// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MenuBase.h"

#include "MenuButton.h"

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

bool UMenuBase::GetMenuMoveDirection(const FKey& Key, EMenuMoveDirection& OutMenuMoveDirection)
{
	static const TArray<TPair<TFunction<bool(const FKey&)>, EMenuMoveDirection>> FuncAndDirectionPairs =
	{
		{ IsUpKey, EMenuMoveDirection::Up },
		{ IsDownKey, EMenuMoveDirection::Down },
		{ IsLeftKey, EMenuMoveDirection::Left },
		{ IsRightKey, EMenuMoveDirection::Right }
	};

	for (const auto& [Func, Direction] : FuncAndDirectionPairs)
	{
		if (Func(Key))
		{
			OutMenuMoveDirection = Direction;
			return true;
		}
	}

	return false;
}

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
	if (EMenuMoveDirection MenuMoveDirection = EMenuMoveDirection::None;
		UMenuBase::GetMenuMoveDirection(Key, MenuMoveDirection))
	{
		const int32 MoveDelta = UMenuBase::GetMenuMoveDelta(MenuMoveDirection);
		MoveMenuButtonFocus(MoveDelta);
		return FReply::Handled();
	}

	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
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
