// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisWidgetHudBase.h"
#include "TetrisWidgetHudIngame.generated.h"

USTRUCT()
struct FHudIngameUpdateDisplayParams
{
	GENERATED_USTRUCT_BODY()

	FHudIngameUpdateDisplayParams()
		: FHudIngameUpdateDisplayParams(0, 0, 0)
	{
	}

	FHudIngameUpdateDisplayParams(const int32 InLevel, const int32 InLineClear, const int32 InLineClearGoal)
		: Level(InLevel)
		, LineClear(InLineClear)
		, LineClearGoal(InLineClearGoal)
	{
	}

	int32 Level;
	int32 LineClear;
	int32 LineClearGoal;
};

/**
 * 
 */
UCLASS()
class TETRIS_API UTetrisWidgetHudIngame : public UTetrisWidgetHudBase
{
	GENERATED_BODY()
	
public:
	void InitializeDisplay(const FHudIngameUpdateDisplayParams& DisplayParams);

	void UpdateDisplay(const FHudIngameUpdateDisplayParams& DisplayParams);
	void UpdateTimeDisplay(const float NewTime);

private:
	void UpdateLevelDisplay(const int32 NewLevel);
	void UpdateLineClearDisplay(const int32 NewLineClear, const int32 NewLineClearGoal);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlockBase> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlockBase> LineClearText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlockBase> TimeText;
};
