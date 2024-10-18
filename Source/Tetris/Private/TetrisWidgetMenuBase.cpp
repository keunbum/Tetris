// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisWidgetMenuBase.h"

#include "MenuButton.h"

bool UTetrisWidgetMenuBase::GetMenuMoveDirection(const FKey& Key, EMenuMoveDirection& OutMenuMoveDirection)
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

FName UTetrisWidgetMenuBase::GetMenuMoveDirectionName(const EMenuMoveDirection MenuMoveDirection)
{
	switch (MenuMoveDirection)
	{
	case EMenuMoveDirection::Up:
		return TEXT("Up");
	case EMenuMoveDirection::Down:
		return TEXT("Down");
	case EMenuMoveDirection::Left:
		return TEXT("Left");
	case EMenuMoveDirection::Right:
		return TEXT("Right");
	default:
		checkNoEntry();
		return NAME_None;
	}
}

void UTetrisWidgetMenuBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsFocusable = true;
}

void UTetrisWidgetMenuBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetInitialFocus();
}

FReply UTetrisWidgetMenuBase::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsNoButtonFocused())
	{
		SetDefaultMenuButtonFocus();
		return FReply::Handled();
	}

	// if this widget has no keyboard focus, then set keyboard focus to current button index.
	if (UMenuButton* const MenuButton = MenuButtons[FocusedButtonIndex];
		MenuButton && !MenuButton->HasKeyboardFocus())
	{
		MenuButton->SetKeyboardFocus();
		return FReply::Handled();
	}

	const FKey Key = InKeyEvent.GetKey();
	if (EMenuMoveDirection MenuMoveDirection = EMenuMoveDirection::None;
		UTetrisWidgetMenuBase::GetMenuMoveDirection(Key, MenuMoveDirection))
	{
		const int32 MoveDelta = UTetrisWidgetMenuBase::GetMenuMoveDelta(MenuMoveDirection);
		MoveMenuButtonFocus(MoveDelta);
		return FReply::Handled();
	}

	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UTetrisWidgetMenuBase::SetDefaultMenuButtonFocus()
{
	SetMenuButtonFocusByButtonIndex(DefaultFocusedButtonIndex);
}

void UTetrisWidgetMenuBase::SetWidgetFocusOnly()
{
	// 초기에 포커싱된 버튼 없음.
	FocusedButtonIndex = UTetrisWidgetMenuBase::InvalidButtonIndex;
	// 위젯 자체에는 포커싱이 걸려야 키보드 입력을 받을 수 있음.
	SetFocus();
}

void UTetrisWidgetMenuBase::SetMenuButtonFocusByButtonIndex(const int32 NewFocusedButtonIndex)
{
	FocusedButtonIndex = NewFocusedButtonIndex;
	if (UMenuButton* const MenuButton = MenuButtons[FocusedButtonIndex])
	{
		MenuButton->SetFocus();
	}
}

void UTetrisWidgetMenuBase::MoveMenuButtonFocus(const int32 MoveDelta)
{
	const int32 NewFocusedButtonIndex = (FocusedButtonIndex + MoveDelta + MenuButtons.Num()) % MenuButtons.Num();
	SetMenuButtonFocusByButtonIndex(NewFocusedButtonIndex);
}

void UTetrisWidgetMenuBase::SetInitialFocus()
{
	SetWidgetFocusOnly();
}
