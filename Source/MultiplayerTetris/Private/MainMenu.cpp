// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenu.h"

#include "MenuButton.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UMainMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FReply Reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	UE_LOG(LogTemp, Display, TEXT("MainMenu::NativeOnKeyDown() - Key: %s"), *InKeyEvent.GetKey().ToString());
	return Reply;
}
