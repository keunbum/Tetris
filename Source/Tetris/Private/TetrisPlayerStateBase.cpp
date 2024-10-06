// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerStateBase.h"

#include "TetrisInGameGameMode.h"
#include "GoalSystemInterface.h"
#include "HUDSingle.h"

ATetrisPlayerStateBase::ATetrisPlayerStateBase()
	: GameLevel(ATetrisIngameGameMode::DefaultGameLevel)
	, LineClearCount(ATetrisIngameGameMode::DefaultLineCount)
{
	SetScore(ATetrisIngameGameMode::DefaultScore);
}

void ATetrisPlayerStateBase::Initialize(const IGoalSystemInterface* GoalSystem)
{
	const int32 InitialLineClearGoal = GoalSystem->GetLevelUpLineCountGoal(GetGameLevel());
	SetLineClearGoal(InitialLineClearGoal);
	DebugPrint();
}

void ATetrisPlayerStateBase::LevelUp(const IGoalSystemInterface* GoalSystem)
{
	AddGameLevel(1);

	AddTotalLineClearCount(GetLineClearCount());
	SetLineClearCount(0);

	const int32 OldGoalLineClear = GetLineClearGoal(); // may be negative
	const int32 NewLevelUpLineCountGoal = GoalSystem->GetLevelUpLineCountGoal(GetGameLevel());
	const int32 NewGoalLineClear = OldGoalLineClear + NewLevelUpLineCountGoal;
	check(NewGoalLineClear > 0);
	SetLineClearGoal(NewGoalLineClear);
}

FHUDSingleUpdateDisplayParams ATetrisPlayerStateBase::GetHUDSingleUpdateDisplayParams() const
{
	return FHUDSingleUpdateDisplayParams(GetGameLevel(), GetLineClearGoal());
}

void ATetrisPlayerStateBase::UpdateState(const FTetrisGamePlayInfo& PlayInfo)
{
	const int32 ClearedLineCount = PlayInfo.HitList.Num();
	AddLineClearCount(ClearedLineCount);
	SubtractLineClearGoal(ClearedLineCount);
}

void ATetrisPlayerStateBase::DebugPrint() const
{
	UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerStateBase::DebugPrint()"));
	UE_LOG(LogTemp, Display, TEXT("GameLevel: %d"), GetGameLevel());
	UE_LOG(LogTemp, Display, TEXT("LineClearCount: %d"), GetLineClearCount());
	UE_LOG(LogTemp, Display, TEXT("TotalLineClearCount: %d"), GetTotalLineClearCount());
	UE_LOG(LogTemp, Display, TEXT("LineClearGoal: %d"), GetLineClearGoal());
}
