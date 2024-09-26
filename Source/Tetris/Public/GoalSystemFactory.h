// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class IGoalSystemInterface;

UENUM()
enum class EGoalSystemType : uint8
{
	None,
	Fixed,
	Variable,
};

/**
 * 
 */
class TETRIS_API GoalSystemFactory
{
public:
	// 생성자 호출 방지
	GoalSystemFactory() = delete;

	static IGoalSystemInterface* CreateGoalSystemInterface(const EGoalSystemType GoalSystemType, UObject* const Outer);
};
