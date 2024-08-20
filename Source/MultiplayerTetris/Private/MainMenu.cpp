// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenu.h"

void UMainMenu::SetInitialFocus()
{
	// 초기에 포커싱된 버튼 없음.
	FocusedButtonIndex = UMenuWidgetBase::InvalidButtonIndex;
	// 위젯 자체에는 포커싱이 걸려야 키보드 입력을 받을 수 있음.
	bIsFocusable = true;
	SetFocus();
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeMenuButtons({ StartGameButton, ExitGameButton });
}
