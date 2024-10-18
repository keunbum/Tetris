// Copyright Ryu KeunBeom. All Rights Reserved.


#include "GoalSystemInterface.h"

#include "TetrisPlayerState.h"

// Add default functionality here for any IGoalSystemInterface functions that are not pure virtual.

bool IGoalSystemInterface::IsLevelUpCondition(const ATetrisPlayerState& TetrisPlayerStateBase) const
{
	return TetrisPlayerStateBase.GetLineClearCount() >= GetLevelUpLineCountGoal(TetrisPlayerStateBase.GetGameLevel());
}
