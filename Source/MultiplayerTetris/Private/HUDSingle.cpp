// Copyright Ryu KeunBeom. All Rights Reserved.

#include "HUDSingle.h"

#include "Components/TextBlock.h"

void UHUDSingle::UpdateLevelDisplay(const int32 NewLevel)
{
	check(LevelText != nullptr);
	LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level: %2d"), NewLevel)));
}
