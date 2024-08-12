// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenu.h"

#include "MenuButton.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	MenuButtons = { StartGameButton, ExitGameButton };
	for (const UMenuButton* MenuButton : MenuButtons)
	{
		check(MenuButton != nullptr);
	}

	// 초기에 포커싱된 버튼 없음.
	FocusedButtonIndex = -1;
	// 이 위젯 자체는 포커싱되어야 사용자 입력에 대응할 수 있음.
	bIsFocusable = true;
	SetFocus();
}

FReply UMainMenu::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("UMainMenu::NativeOnPreviewKeyDown() - Key: %s"), *InKeyEvent.GetKey().ToString());

	const bool bNoButtonFocused = (FocusedButtonIndex == -1);
	if (bNoButtonFocused)
	{
		SetInitialMenuButtonFocus();
		return FReply::Handled();
	}

	const FKey Key = InKeyEvent.GetKey();
	const bool bIsUpKey = (Key == EKeys::Up || Key == EKeys::Gamepad_DPad_Up || Key == EKeys::Gamepad_LeftStick_Up);
	const bool bIsDownKey = (Key == EKeys::Down || Key == EKeys::Gamepad_DPad_Down || Key == EKeys::Gamepad_LeftStick_Down);
	const bool bIsMenuMoveKey = bIsUpKey || bIsDownKey;
	if (bIsMenuMoveKey)
	{
		const int32 Delta = bIsUpKey ? -1 : 1;
		MoveMenuButtonFocus(Delta);
		return FReply::Handled();
	}

	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UMainMenu::SetInitialMenuButtonFocus()
{
	FocusedButtonIndex = 0;
	MenuButtons[FocusedButtonIndex]->SetFocus();
}

void UMainMenu::MoveMenuButtonFocus(const int32 Delta)
{
	FocusedButtonIndex = (FocusedButtonIndex + Delta + MenuButtons.Num()) % MenuButtons.Num();
	MenuButtons[FocusedButtonIndex]->SetFocus();
}
