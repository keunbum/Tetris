// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MenuButton.h"

UMenuButton::UMenuButton()
{
	static FSlateBrush TransparentBrush;
	TransparentBrush.TintColor = FSlateColor(FLinearColor::Transparent);
	TransparentBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::HalfHeightRadius;
	TransparentBrush.OutlineSettings.Color = FSlateColor(FLinearColor::Transparent);

	WidgetStyle.SetNormal(TransparentBrush);
}
