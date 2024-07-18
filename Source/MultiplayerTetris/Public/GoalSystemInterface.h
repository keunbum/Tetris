// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GoalSystemInterface.generated.h"

class ATetrisPlayerStateBase;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGoalSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERTETRIS_API IGoalSystemInterface
{
	GENERATED_BODY()

public:
	bool IsLevelUpCondition(const ATetrisPlayerStateBase& TetrisPlayerStateBase) const;
	virtual int32 GetLevelUpLineCountGoal(const int32 GameLevel) const = 0;
};
