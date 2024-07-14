// Copyright Ryu KeunBeom. All Rights Reserved.

#include "GoalSystemFactory.h"

#include "UObject/NoExportTypes.h"

#include "GoalSystemInterface.h"
#include "FixedGoalSystem.h"
#include "VariableGoalSystem.h"

IGoalSystemInterface* GoalSystemFactory::CreateGoalSystem(const EGoalSystemType GoalSystemType, UObject* const Outer)
{
	switch (GoalSystemType)
	{
	case EGoalSystemType::Fixed:
		return NewObject<UFixedGoalSystem>(Outer);
	case EGoalSystemType::Variable:
		return NewObject<UVariableGoalSystem>(Outer);
	default:
		return nullptr;
	}
}
