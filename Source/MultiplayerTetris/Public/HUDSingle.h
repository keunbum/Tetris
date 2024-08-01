// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUDBase.h"
#include "HUDSingle.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UHUDSingle : public UHUDBase
{
	GENERATED_BODY()
	
public:
	void InitializeHUD(const ATetrisPlayerStateBase* PlayerState);

	void UpdateDisplay(const ATetrisPlayerStateBase* PlayerState);

private:
	void UpdateLevelDisplay(const int32 NewLevel);
	void UpdateGoalDisplay(const int32 NewGoal);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoalText;
};
