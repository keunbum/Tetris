// Copyright Ryu KeunBeom. All Rights Reserved.

#include "HUDSingle.h"

#include "Components/TextBlock.h"

#include "TetrisPlayerStateBase.h"

void UHUDSingle::InitializeHUD(ATetrisPlayerStateBase* const NewPlayerState)
{
	check(NewPlayerState != nullptr);
	PlayerState = NewPlayerState;

	UpdateDisplay();
	AddToViewport();
}

void UHUDSingle::UpdateDisplay()
{
	UpdateLevelDisplay(PlayerState->GetGameLevel());
}

void UHUDSingle::UpdateLevelDisplay(const int32 NewLevel)
{
	check(LevelText != nullptr);
	const FString LevelString = FString::Printf(TEXT("%-15s %10d"), TEXT("Level"), NewLevel);
	LevelText->SetText(FText::FromString(LevelString));
}
