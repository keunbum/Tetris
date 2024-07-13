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
	virtual bool IsLevelUpCondition(const ATetrisPlayerStateBase& TetrisPlayerStateBase) const override;

private:
	static constexpr int32 LevelUpLineCountGoal = 10;
};
