// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "GoalSystemInterface.h"

#include "VariableGoalSystem.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UVariableGoalSystem : public UObject, public IGoalSystemInterface
{
	GENERATED_BODY()
	
public:
	virtual int32 GetLevelUpLineCountGoal(const int32 GameLevel) const override { return GameLevel * LevelUpLineCountGoalMultiplier; }

private:
	static constexpr int32 LevelUpLineCountGoalMultiplier = 5;
};
