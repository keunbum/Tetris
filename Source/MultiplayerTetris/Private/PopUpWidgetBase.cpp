// Copyright Ryu KeunBeom. All Rights Reserved.

#include "PopUpWidgetBase.h"
#include "ButtonBase.h"

void UPopUpWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (!CloseButton->OnClicked.IsBound())
	{
		CloseButton->OnClicked.AddDynamic(this, &UPopUpWidgetBase::OnCloseClicked);
	}
}

void UPopUpWidgetBase::OnCloseClicked()
{
	PrepareClose();
	RemoveFromParent();
}
