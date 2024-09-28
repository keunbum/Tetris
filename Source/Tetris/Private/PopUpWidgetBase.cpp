// Copyright Ryu KeunBeom. All Rights Reserved.

#include "PopUpWidgetBase.h"
#include "ButtonBase.h"

void UPopUpWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UPopUpWidgetBase::OnCloseClicked);
	}
}

void UPopUpWidgetBase::OnCloseClicked()
{
	RemoveFromParent();
}
