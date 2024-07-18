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

void ATetrisPlayerStateBase::LevelUp(const int32 LevelUpLineCountGoal)
{
	AddGameLevel(1);

	AddLineClearCount(-LevelUpLineCountGoal);
	AddTotalLineClearCount(LevelUpLineCountGoal);
}

void ATetrisPlayerStateBase::UpdateState(const FTetrisGamePlayInfo& PlayInfo)
{
	const int32 LineCleared = PlayInfo.HitList.Num();
	AddLineClearCount(LineCleared);
	UE_LOG(LogTemp, Warning, TEXT("LineClearCount: %d"), GetLineClearCount());
}
