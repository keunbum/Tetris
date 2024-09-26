// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUDBase.h"
#include "HUDSingle.generated.h"

USTRUCT()
struct FHUDSingleUpdateDisplayParams
{
	GENERATED_USTRUCT_BODY()

	FHUDSingleUpdateDisplayParams()
		: FHUDSingleUpdateDisplayParams(0, 0)
	{
	}

	FHUDSingleUpdateDisplayParams(const int32 InLevel, const int32 InGoal)
		: Level(InLevel)
		, Goal(InGoal)
	{
	}

	int32 Level;
	int32 Goal;
};

/**
 * 
 */
UCLASS()
class TETRIS_API UHUDSingle : public UHUDBase
{
	GENERATED_BODY()
	
public:
	void InitializeHUD(const FHUDSingleUpdateDisplayParams& DisplayParams, ATetrisInGameGameMode* const InTetrisGameMode);

	void UpdateDisplay(const FHUDSingleUpdateDisplayParams& DisplayParams);

private:
	void UpdateLevelDisplay(const int32 NewLevel);
	void UpdateGoalDisplay(const int32 NewGoal);
	void UpdateTimeDisplay(const float NewTime);

	void OnUpdateTime();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoalText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	FTimerHandle UpdateTimeTimerHandle;
};
