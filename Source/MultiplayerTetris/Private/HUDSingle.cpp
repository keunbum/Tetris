// Copyright Ryu KeunBeom. All Rights Reserved.

#include "HUDSingle.h"

#include "Components/TextBlock.h"

#include "TetrisPlayerStateBase.h"

void UHUDSingle::InitializeHUD(const ATetrisPlayerStateBase* PlayerState)
{
	UpdateDisplay(PlayerState);
	AddToViewport();
}

void UHUDSingle::UpdateDisplay(const ATetrisPlayerStateBase* PlayerState)
{
	UpdateLevelDisplay(PlayerState->GetGameLevel());
}

void UHUDSingle::UpdateLevelDisplay(const int32 NewLevel)
{
	check(LevelText != nullptr);
	const FString LevelString = FString::Printf(TEXT("%-15s %10d"), TEXT("Level"), NewLevel);
	LevelText->SetText(FText::FromString(LevelString));
}

void UHUDSingle::UpdateGoalDisplay(const int32 NewGoal)
{
	check(GoalText != nullptr);
	const FString GoalString = FString::Printf(TEXT("%-15s %10d"), TEXT("Goal"), NewGoal);
	GoalText->SetText(FText::FromString(GoalString));
}
