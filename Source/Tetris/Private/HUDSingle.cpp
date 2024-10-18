// Copyright Ryu KeunBeom. All Rights Reserved.

#include "HUDSingle.h"

#include "TextBlockBase.h"
#include "Engine/World.h"

#include "TetrisGameModeIngameBase.h"

void UHUDSingle::InitializeHUD(const FHUDSingleUpdateDisplayParams& DisplayParams, ATetrisGameModeIngameBase* const InTetrisGameMode)
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
	UpdateLineClearDisplay(DisplayParams.LineClear, DisplayParams.LineClearGoal);
}

void UHUDSingle::UpdateLevelDisplay(const int32 NewLevel)
{
	static constexpr wchar_t Name[] = TEXT("Level");
	const FString LevelString = FString::Printf(TEXT("%d"), NewLevel);
	UHUDBase::UpdateTextBlockWithNameAndValue(LevelText, Name, LevelString);
}

void UHUDSingle::UpdateLineClearDisplay(const int32 NewLineClear, const int32 NewLineClearGoal)
{
	static constexpr wchar_t Name[] = TEXT("Line Clear");
	const FString LineClearValueString = FString::Printf(TEXT("%d / %d"), NewLineClear, NewLineClearGoal);
	UHUDBase::UpdateTextBlockWithNameAndValue(LineClearText, Name, LineClearValueString);
}

void UHUDSingle::UpdateTimeDisplay(const float NewTime)
{
	static constexpr wchar_t Name[] = TEXT("Time");
	const FString ElapsedTimeString = UHUDBase::GetFormattedTimeString(NewTime);
	UHUDBase::UpdateTextBlockWithNameAndValue(TimeText, Name, ElapsedTimeString);
}

void UHUDSingle::OnUpdateTime()
{
	if (TetrisGameMode)
	{
		UpdateTimeDisplay(TetrisGameMode->GetElapsedTime());
	}
}
