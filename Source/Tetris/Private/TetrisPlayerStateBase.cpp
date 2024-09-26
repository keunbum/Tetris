// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerStateBase.h"

#include "TetrisInGameGameMode.h"
#include "GoalSystemInterface.h"
#include "HUDSingle.h"

ATetrisPlayerStateBase::ATetrisPlayerStateBase()
	: GameLevel(ATetrisInGameGameMode::DefaultGameLevel)
	, LineClearCount(ATetrisInGameGameMode::DefaultLineCount)
{
	SetScore(ATetrisInGameGameMode::DefaultScore);
}

void ATetrisPlayerStateBase::Initialize(const IGoalSystemInterface* GoalSystem)
{
	const int32 InitialGoalLineClear = GoalSystem->GetLevelUpLineCountGoal(GetGameLevel());
	SetGoalLineClear(InitialGoalLineClear);
	DebugPrint();
}

void ATetrisPlayerStateBase::LevelUp(const IGoalSystemInterface* GoalSystem)
{
	AddGameLevel(1);

	AddTotalLineClearCount(GetLineClearCount());
	SetLineClearCount(0);

	const int32 OldGoalLineClear = GetGoalLineClear(); // may be negative
	const int32 NewLevelUpLineCountGoal = GoalSystem->GetLevelUpLineCountGoal(GetGameLevel());
	const int32 NewGoalLineClear = OldGoalLineClear + NewLevelUpLineCountGoal;
	check(NewGoalLineClear > 0);
	SetGoalLineClear(NewGoalLineClear);
}

FHUDSingleUpdateDisplayParams ATetrisPlayerStateBase::GetHUDSingleUpdateDisplayParams() const
{
	return FHUDSingleUpdateDisplayParams(GetGameLevel(), GetGoalLineClear());
}

void ATetrisPlayerStateBase::UpdateState(const FTetrisGamePlayInfo& PlayInfo)
{
	const int32 ClearedLineCount = PlayInfo.HitList.Num();
	AddLineClearCount(ClearedLineCount);
	SubtractGoalLineClear(ClearedLineCount);
}

void ATetrisPlayerStateBase::DebugPrint() const
{
	UE_LOG(LogTemp, Warning, TEXT("GameLevel: %d"), GetGameLevel());
	UE_LOG(LogTemp, Warning, TEXT("LineClearCount: %d"), GetLineClearCount());
	UE_LOG(LogTemp, Warning, TEXT("TotalLineClearCount: %d"), GetTotalLineClearCount());
	UE_LOG(LogTemp, Warning, TEXT("GoalLineClear: %d"), GetGoalLineClear());
}