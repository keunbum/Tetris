// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuWidget.h"

void UMainMenuWidget::SetInitialFocus()
{
	SetWidgetFocusOnly();
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeMenuButtons({ StartGameButton, ExitGameButton });
}
