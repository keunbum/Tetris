// Copyright Ryu KeunBeom. All Rights Reserved.

#include "HUDSingle.h"

void UHUDSingle::InitializeHUD(const FHUDSingleUpdateDisplayParams& DisplayParams)
{
	UpdateDisplay(DisplayParams);
}

void UHUDSingle::UpdateDisplay(const FHUDSingleUpdateDisplayParams& DisplayParams)
{
	UpdateLevelDisplay(DisplayParams.Level);
	UpdateLineClearDisplay(DisplayParams.LineClear, DisplayParams.LineClearGoal);
}

void UHUDSingle::UpdateTimeDisplay(const float NewTime)
{
	static constexpr TCHAR Name[] = TEXT("Time");
	const FString ElapsedTimeString = UHUDBase::GetFormattedTimeString(NewTime);
	UHUDBase::UpdateTextBlockWithNameAndValue(TimeText, Name, ElapsedTimeString);
}

void UHUDSingle::UpdateLevelDisplay(const int32 NewLevel)
{
	static constexpr TCHAR Name[] = TEXT("Level");
	const FString LevelString = FString::Printf(TEXT("%d"), NewLevel);
	UHUDBase::UpdateTextBlockWithNameAndValue(LevelText, Name, LevelString);
}

void UHUDSingle::UpdateLineClearDisplay(const int32 NewLineClear, const int32 NewLineClearGoal)
{
	static constexpr TCHAR Name[] = TEXT("Line Clear");
	const FString LineClearValueString = FString::Printf(TEXT("%d / %d"), NewLineClear, NewLineClearGoal);
	UHUDBase::UpdateTextBlockWithNameAndValue(LineClearText, Name, LineClearValueString);
}
