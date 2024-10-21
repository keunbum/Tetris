// Copyright Ryu KeunBeom. All Rights Reserved.


#include "TetrisWidgetHudIngame.h"

void UTetrisWidgetHudIngame::InitializeDisplay(const FHudIngameUpdateDisplayParams& DisplayParams)
{
	UpdateDisplay(DisplayParams);
}

void UTetrisWidgetHudIngame::UpdateDisplay(const FHudIngameUpdateDisplayParams& DisplayParams)
{
	UpdateLevelDisplay(DisplayParams.Level);
	UpdateLineClearDisplay(DisplayParams.LineClear, DisplayParams.LineClearGoal);
}

void UTetrisWidgetHudIngame::UpdateTimeDisplay(const float NewTime)
{
	static constexpr TCHAR Name[] = TEXT("Time");
	const FString ElapsedTimeString = UTetrisWidgetHudBase::GetFormattedTimeString(NewTime);
	UTetrisWidgetHudBase::UpdateTextBlockWithNameAndValue(TimeText, Name, ElapsedTimeString);
}

void UTetrisWidgetHudIngame::UpdateLevelDisplay(const int32 NewLevel)
{
	static constexpr TCHAR Name[] = TEXT("Level");
	const FString LevelString = FString::Printf(TEXT("%d"), NewLevel);
	UTetrisWidgetHudBase::UpdateTextBlockWithNameAndValue(LevelText, Name, LevelString);
}

void UTetrisWidgetHudIngame::UpdateLineClearDisplay(const int32 NewLineClear, const int32 NewLineClearGoal)
{
	static constexpr TCHAR Name[] = TEXT("Line Clear");
	const FString LineClearValueString = FString::Printf(TEXT("%d / %d"), NewLineClear, NewLineClearGoal);
	UTetrisWidgetHudBase::UpdateTextBlockWithNameAndValue(LineClearText, Name, LineClearValueString);
}