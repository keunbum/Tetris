// Copyright Ryu KeunBeom. All Rights Reserved.

#include "GoalSystemFactory.h"

#include "UObject/NoExportTypes.h"

#include "GoalSystemInterface.h"
#include "GoalSystemFixed.h"
#include "GoalSystemVariable.h"

IGoalSystemInterface* GoalSystemFactory::CreateGoalSystemInterface(const EGoalSystemType GoalSystemType, UObject* const Outer)
{
	switch (GoalSystemType)
	{
	case EGoalSystemType::Fixed:
		return NewObject<UGoalSystemFixed>(Outer);
	case EGoalSystemType::Variable:
		return NewObject<UGoalSystemVariable>(Outer);
	default:
		return nullptr;
	}
}
