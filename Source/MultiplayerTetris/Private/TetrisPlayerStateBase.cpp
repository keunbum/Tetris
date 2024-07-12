// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerStateBase.h"

#include "TetrisGameModeBase.h"

ATetrisPlayerStateBase::ATetrisPlayerStateBase()
	: GameLevel(ATetrisGameModeBase::DefaultGameLevel)
	, LineCount(ATetrisGameModeBase::DefaultLineCount)
{
	SetScore(ATetrisGameModeBase::DefaultScore);
}
