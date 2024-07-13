// Copyright Ryu KeunBeom. All Rights Reserved.


#include "FixedGoalSystem.h"

#include "TetrisPlayerStateBase.h"

bool UFixedGoalSystem::IsLevelUpCondition(const ATetrisPlayerStateBase& TetrisPlayerStateBase) const
{
	return TetrisPlayerStateBase.GetLineClearCount() >= UFixedGoalSystem::LevelUpLineCountGoal;
}
