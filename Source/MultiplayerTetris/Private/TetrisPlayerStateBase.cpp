// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerStateBase.h"

#include "TetrisGameModeBase.h"
#include "GoalSystemInterface.h"

ATetrisPlayerStateBase::ATetrisPlayerStateBase()
	: GameLevel(ATetrisGameModeBase::DefaultGameLevel)
	, LineClearCount(ATetrisGameModeBase::DefaultLineCount)
{
	SetScore(ATetrisGameModeBase::DefaultScore);
}

void ATetrisPlayerStateBase::Initialize(const IGoalSystemInterface* GoalSystem)
{
	const int32 InitialGoalLineClear = GoalSystem->GetLevelUpLineCountGoal(GetGameLevel());
	SetGoalLineClear(InitialGoalLineClear);
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

void ATetrisPlayerStateBase::UpdateState(const FTetrisGamePlayInfo& PlayInfo)
{
	const int32 ClearedLineCount = PlayInfo.HitList.Num();
	UE_LOG(LogTemp, Warning, TEXT("ClearedLineCount: %d"), ClearedLineCount);
	AddLineClearCount(ClearedLineCount);
	SubtractGoalLineClear(ClearedLineCount);
}
