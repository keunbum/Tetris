// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenu.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeMenuButtons({ StartGameButton, ExitGameButton });

	InitialFocusedButtonIndex = 0;
	// 이 위젯 자체는 포커싱되어야 함.
	bIsFocusable = true;
	SetInitialFocus();
}

}
