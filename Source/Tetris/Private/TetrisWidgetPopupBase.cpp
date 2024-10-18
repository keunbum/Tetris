// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisWidgetPopupBase.h"
#include "ButtonBase.h"

void UTetrisWidgetPopupBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UTetrisWidgetPopupBase::OnCloseClicked);
	}

	bIsFocusable = true;
}

void UTetrisWidgetPopupBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetFocus();
}

void UTetrisWidgetPopupBase::OnCloseClicked()
{
	RemoveFromParent();
}
