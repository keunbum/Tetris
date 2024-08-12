// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenu.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeMenuButtons({ StartGameButton, ExitGameButton });

	// 이 위젯 자체는 포커싱되어야 함.
	bIsFocusable = true;
	SetInitialFocus();
}

FReply UMainMenu::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
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

void UMainMenu::SetInitialFocus()
{
	// 초기에 포커싱된 버튼 없음.
	FocusedButtonIndex = UMenuBase::InvalidButtonIndex;
	// 위젯 자체에는 포커싱이 걸려야 키보드 입력을 받을 수 있음.
	SetFocus();
}

void UMainMenu::SetInitialMenuButtonFocus()
{
	SetMenuButtonFocus(UMainMenu::InitialFocusedButtonIndex);
}
