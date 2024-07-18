// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerStateBase.h"

#include "TetrisGameModeBase.h"

ATetrisPlayerStateBase::ATetrisPlayerStateBase()
	: GameLevel(ATetrisGameModeBase::DefaultGameLevel)
	, LineClearCount(ATetrisGameModeBase::DefaultLineCount)
{
	SetScore(ATetrisGameModeBase::DefaultScore);
}

void ATetrisPlayerStateBase::LevelUp(const int32 LevelUpLineCountGoal)
{
	AddGameLevel(1);

	AddLineClearCount(-LevelUpLineCountGoal);
	AddTotalLineClearCount(LevelUpLineCountGoal);
	UE_LOG(LogTemp, Warning, TEXT("Total LineClearCount: %d"), GetTotalLineClearCount());
}

void ATetrisPlayerStateBase::UpdateState(const FTetrisGamePlayInfo& PlayInfo)
{
	AddLineClearCount(PlayInfo.HitList.Num());
	UE_LOG(LogTemp, Warning, TEXT("LineClearCount: %d"), GetLineClearCount());
}
