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

	bIsFocusable = true;
}

void UPopUpWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetFocus();
}

void UPopUpWidgetBase::OnCloseClicked()
{
	RemoveFromParent();
}
