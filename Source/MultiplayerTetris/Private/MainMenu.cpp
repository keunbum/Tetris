// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenu.h"

#include "MenuButton.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize button array
	const TArray<UMenuButton*> Buttons = { StartGameButton, ExitGameButton };
	const int32 InitialFocusedButtonIndex = 0;

	for (UMenuButton* const Button : Buttons)
	{
		if (Button)
		{
			static FSlateBrush TransparentBrush;
			TransparentBrush.TintColor = FSlateColor(FLinearColor::Transparent);
			TransparentBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::HalfHeightRadius;
			TransparentBrush.OutlineSettings.Color = FSlateColor(FLinearColor::Transparent);

			FButtonStyle& ButtonStyle = Button->WidgetStyle;
			ButtonStyle.SetNormal(TransparentBrush);
		}
	}

	// Set initial focus
	if (Buttons.IsValidIndex(InitialFocusedButtonIndex))
	{
		UButton* const FocusedButton = Buttons[InitialFocusedButtonIndex];
		UE_LOG(LogTemp, Display, TEXT("Focused Button: %s"), *FocusedButton->GetFullName());
		FocusedButton->SetFocus();
	}

	UE_LOG(LogTemp, Warning, TEXT("MainMenu::NativeConstruct() - Initialized"));
}

FReply UMainMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FReply Reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	UE_LOG(LogTemp, Display, TEXT("MainMenu::NativeOnKeyDown() - Key: %s"), *InKeyEvent.GetKey().ToString());
	return Reply;
}
