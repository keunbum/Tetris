// Copyright Ryu KeunBeom. All Rights Reserved.

#include "HUDSingle.h"

#include "Components/TextBlock.h"
#include "Engine/World.h"

#include "TetrisInGameGameMode.h"

void UHUDSingle::InitializeHUD(const FHUDSingleUpdateDisplayParams& DisplayParams, ATetrisInGameGameMode* const InTetrisGameMode)
{
	UpdateDisplay(DisplayParams);
	TetrisGameMode = InTetrisGameMode;
	AddToViewport();

	// 경과 시간 업데이트 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(UpdateTimeTimerHandle, this, &UHUDSingle::OnUpdateTime, UHUDBase::TimeDisplayUpdateInterval, /* InbLoop */ true, /* InFirstDelay */ 0.0f);
}

void UHUDSingle::UpdateDisplay(const FHUDSingleUpdateDisplayParams& DisplayParams)
{
	UpdateLevelDisplay(DisplayParams.Level);
	UpdateGoalDisplay(DisplayParams.Goal);
}

void UHUDSingle::UpdateLevelDisplay(const int32 NewLevel)
{
	if (LevelText)
	{
		const FString LevelString = FString::Printf(TEXT("%-15s %10d"), TEXT("Level"), NewLevel);
		LevelText->SetText(FText::FromString(LevelString));
	}
}

void UHUDSingle::UpdateGoalDisplay(const int32 NewGoal)
{
	if (GoalText)
	{
		const FString GoalString = FString::Printf(TEXT("%-15s %10d"), TEXT("Goal"), NewGoal);
		GoalText->SetText(FText::FromString(GoalString));
	}
}

void UHUDSingle::UpdateTimeDisplay(const float NewTime)
{
	if (TimeText)
	{
		const FString ElapsedTimeString = UHUDBase::GetFormattedTimeString(NewTime);
		const FString TimeString = FString::Printf(TEXT("%-10s %10s"), TEXT("Time"), *ElapsedTimeString);
		TimeText->SetText(FText::FromString(TimeString));
	}
}

void UHUDSingle::OnUpdateTime()
{
	if (TetrisGameMode)
	{
		UpdateTimeDisplay(TetrisGameMode->GetElapsedTime());
	}
}