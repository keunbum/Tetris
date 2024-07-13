// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerStateBase.h"

#include "TetrisGameModeBase.h"

ATetrisPlayerStateBase::ATetrisPlayerStateBase()
	: GameLevel(ATetrisGameModeBase::DefaultGameLevel)
	, LineClearCount(ATetrisGameModeBase::DefaultLineCount)
{
	SetScore(ATetrisGameModeBase::DefaultScore);
}

void ATetrisPlayerStateBase::LevelUp()
{
	AddGameLevel(1);

	SetLineClearCount(ATetrisGameModeBase::DefaultLineCount);

	UE_LOG(LogTemp, Warning, TEXT("Level Up! Current Level: %d"), GetGameLevel());
}

void ATetrisPlayerStateBase::UpdateState(const FTetrisGamePlayInfo& UpdateInfo)
{
	AddLineClearCount(UpdateInfo.HitList.Num());
	AddTotalLineClearCount(UpdateInfo.HitList.Num());

	UE_LOG(LogTemp, Warning, TEXT("Total Cleared LineClearCount: %d"), GetLineClearCount());
}
