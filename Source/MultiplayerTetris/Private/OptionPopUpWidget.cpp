// Copyright Ryu KeunBeom. All Rights Reserved.

#include "OptionPopUpWidget.h"
#include "ButtonBase.h"

void UOptionPopUpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!CloseButton->OnClicked.IsBound())
	{
		CloseButton->OnClicked.AddDynamic(this, &UOptionPopUpWidget::OnCloseClicked);
	}
}

void UOptionPopUpWidget::OnCloseClicked()
{
	// Close this Widget
	RemoveFromParent();
}
