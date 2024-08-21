// Copyright Ryu KeunBeom. All Rights Reserved.

#include "PauseMenuWidget.h"

#include "MenuButton.h"


void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetMenuButtons({ ResumeButton, RestartButton, ExitButton });

	SetDefaultMenuButtonFocus();

	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
	RestartButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnRestartClicked);
	ExitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnExitClicked);
}

{
}

void UPauseMenuWidget::OnRestartClicked()
{

}

void UPauseMenuWidget::OnExitClicked()
{

}
