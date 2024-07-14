// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "GoalSystemInterface.h"

#include "FixedGoalSystem.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UFixedGoalSystem : public UObject, public IGoalSystemInterface
{
	GENERATED_BODY()
	
public:
	virtual int32 GetLevelUpLineCountGoal(const int32) const override { return LevelUpLineCountGoal; }

private:
	static constexpr int32 LevelUpLineCountGoal = 10;
};
