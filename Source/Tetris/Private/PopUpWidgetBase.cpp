// Copyright Ryu KeunBeom. All Rights Reserved.

#include "PopUpWidgetBase.h"
#include "ButtonBase.h"

void UPopupWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UPopupWidgetBase::OnCloseClicked);
	}

	bIsFocusable = true;
}

void UPopupWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetFocus();
}

void UPopupWidgetBase::OnCloseClicked()
{
	RemoveFromParent();
}
