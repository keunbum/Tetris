// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerStateBase.h"

#include "TetrisInGameGameMode.h"
#include "GoalSystemInterface.h"
#include "HUDSingle.h"

ATetrisPlayerStateBase::ATetrisPlayerStateBase()
	: GameLevel(ATetrisIngameGameMode::DefaultGameLevel)
	, LineClearCount(ATetrisIngameGameMode::DefaultLineClearCount)
{
	SetScore(ATetrisIngameGameMode::DefaultScore);
}

void ATetrisPlayerStateBase::Initialize(const IGoalSystemInterface* GoalSystem)
{
	const int32 InitialLineClearGoal = GoalSystem->GetLevelUpLineCountGoal(GetGameLevel());
	SetLineClearGoal(InitialLineClearGoal);
}

void ATetrisPlayerStateBase::LevelUp(const IGoalSystemInterface* GoalSystem)
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

FHUDSingleUpdateDisplayParams ATetrisPlayerStateBase::GetHUDSingleUpdateDisplayParams() const
{
	return FHUDSingleUpdateDisplayParams(GetGameLevel(), GetLineClearCount(), GetLineClearGoal());
}

void ATetrisPlayerStateBase::UpdateState(const FTetrisGamePlayInfo& PlayInfo)
{
	const int32 ClearedLineCount = PlayInfo.HitList.Num();
	AddLineClearCount(ClearedLineCount);
}

void ATetrisPlayerStateBase::DebugPrint() const
{
	UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerStateBase::DebugPrint()"));
	UE_LOG(LogTemp, Display, TEXT("GameLevel: %d"), GetGameLevel());
	UE_LOG(LogTemp, Display, TEXT("LineClearCount: %d"), GetLineClearCount());
	UE_LOG(LogTemp, Display, TEXT("TotalLineClearCount: %d"), GetTotalLineClearCount());
	UE_LOG(LogTemp, Display, TEXT("LineClearGoal: %d"), GetLineClearGoal());
}
