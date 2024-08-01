// Copyright Ryu KeunBeom. All Rights Reserved.

#include "HUDSingle.h"

#include "Components/TextBlock.h"

#include "TetrisGameModeBase.h"

void UHUDSingle::InitializeHUD(const FHUDSingleUpdateDisplayParams& DisplayParams, ATetrisGameModeBase* const InTetrisGameMode)
{
	UpdateDisplay(DisplayParams);
	TetrisGameMode = InTetrisGameMode;
	check(TetrisGameMode != nullptr);
	AddToViewport();
}

void UHUDSingle::UpdateDisplay(const FHUDSingleUpdateDisplayParams& DisplayParams)
{
	UpdateLevelDisplay(DisplayParams.Level);
	UpdateGoalDisplay(DisplayParams.Goal);
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
