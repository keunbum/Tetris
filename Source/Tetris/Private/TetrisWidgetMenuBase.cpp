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
	// 포커싱된 버튼이 없다면 첫 번째(디폴트) 버튼에 포커싱.
	if (IsNoButtonFocused())
	{
		SetDefaultMenuButtonFocus();
		return FReply::Handled();
	}

	// 버튼에 포커싱된 적 있지만, 다른 위젯에 포커싱 되었다가 돌아와 버튼 포커싱이 풀린 경우, 다시 맞춤.
	checkf(FMath::IsWithin(FocusedButtonIndex, 0, MenuButtons.Num()), TEXT("Invalid FocusedButtonIndex: %d"), FocusedButtonIndex);
	if (UMenuButton* const MenuButton = MenuButtons[FocusedButtonIndex];
		MenuButton && !MenuButton->HasKeyboardFocus())
	{
		MenuButton->SetKeyboardFocus();
		return FReply::Handled();
	}

	// 버튼 포커싱 되어 있다면 사용자 입력에 따라 버튼 이동.
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
	checkf(FMath::IsWithin(FocusedButtonIndex, 0, MenuButtons.Num()), TEXT("Invalid FocusedButtonIndex: %d"), FocusedButtonIndex);
	if (UMenuButton* const MenuButton = MenuButtons[FocusedButtonIndex])
	{
		MenuButton->SetFocus();
	}
}

void UTetrisWidgetMenuBase::MoveMenuButtonFocus(const int32 MoveDelta)
{
	// 끝단 버튼에서 이동하면 반대쪽 끝단 버튼으로 이동.
	// 연결 리스트로 구현할 수도 있지만 편의상 배열로 구현.
	const int32 NewFocusedButtonIndex = (FocusedButtonIndex + MoveDelta + MenuButtons.Num()) % MenuButtons.Num();
	SetMenuButtonFocusByButtonIndex(NewFocusedButtonIndex);
}

void UTetrisWidgetMenuBase::SetInitialFocus()
{
	SetWidgetFocusOnly();
}
