// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 목표 시스템 유형을 정의하는 열거형
UENUM()
enum class EGoalSystemType : uint8
{
	Fixed,
};

/**
 * 
 */
class MULTIPLAYERTETRIS_API GoalSystemFactory
{
public:
	// 생성자 호출 방지
	GoalSystemFactory() = delete;
};
