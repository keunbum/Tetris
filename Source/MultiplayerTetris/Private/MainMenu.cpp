// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenu.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeMenuButtons({ StartGameButton, ExitGameButton });
}
