// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerState.h"

#include "TetrisGameModeIngameBase.h"
#include "GoalSystemInterface.h"
#include "HUDSingle.h"
#include "TetrisHudIngameBase.h"

ATetrisPlayerState::ATetrisPlayerState()
	: GameLevel(ATetrisGameModeIngameBase::DefaultGameLevel)
	, LineClearCount(ATetrisGameModeIngameBase::DefaultLineClearCount)
{
	SetScore(ATetrisGameModeIngameBase::DefaultScore);
}

void ATetrisPlayerState::Initialize(const IGoalSystemInterface* GoalSystem)
{
	const int32 InitialLineClearGoal = GoalSystem->GetLevelUpLineCountGoal(GameLevel);
	SetLineClearGoal(InitialLineClearGoal);
}

void ATetrisPlayerState::LevelUp(const IGoalSystemInterface* GoalSystem)
{
	GameLevel += 1;
	AddTotalLineClearCount(LineClearCount);

	const int32 LineClearOver = LineClearCount - LineClearGoal;

	const int32 NewLevelUpLineCountGoal = GoalSystem->GetLevelUpLineCountGoal(GameLevel);
	check(NewLevelUpLineCountGoal > 0);

	const int32 NewLineClearCount = LineClearOver;
	checkf(FMath::IsWithin(NewLineClearCount, 0, NewLevelUpLineCountGoal), TEXT("NewLineClearCount: %d, NewLevelUpLineCountGoal: %d"), NewLineClearCount, NewLevelUpLineCountGoal);

	SetLineClearCount(NewLineClearCount);
	SetLineClearGoal(NewLevelUpLineCountGoal);
}

FHUDSingleUpdateDisplayParams ATetrisPlayerState::GetHUDSingleUpdateDisplayParams() const
{
	return FHUDSingleUpdateDisplayParams(GameLevel, LineClearCount, LineClearGoal);
}

void ATetrisPlayerState::UpdateState(const FTetrisGamePlayInfo& PlayInfo)
{
	const int32 ClearedLineCount = PlayInfo.HitList.Num();
	AddLineClearCount(ClearedLineCount);
}

void ATetrisPlayerState::DebugPrint() const
{
	UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerState::DebugPrint()"));
	UE_LOG(LogTemp, Display, TEXT("GameLevel: %d"), GameLevel);
	UE_LOG(LogTemp, Display, TEXT("LineClearCount: %d"), LineClearCount);
	UE_LOG(LogTemp, Display, TEXT("TotalLineClearCount: %d"), TotalLineClearCount);
	UE_LOG(LogTemp, Display, TEXT("LineClearGoal: %d"), LineClearGoal);
}
