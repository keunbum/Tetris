// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUDBase.h"
#include "HUDSingle.generated.h"

class UTextBlockBase;

USTRUCT()
struct FHUDSingleUpdateDisplayParams
{
	GENERATED_USTRUCT_BODY()

	FHUDSingleUpdateDisplayParams()
		: FHUDSingleUpdateDisplayParams(0, 0, 0)
	{
	}

	FHUDSingleUpdateDisplayParams(const int32 InLevel, const int32 InLineClear, const int32 InLineClearGoal)
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
class TETRIS_API UHUDSingle : public UHUDBase
{
	GENERATED_BODY()
	
public:
	void InitializeHUD(const FHUDSingleUpdateDisplayParams& DisplayParams, ATetrisIngameGameMode* const InTetrisGameMode);

	void UpdateDisplay(const FHUDSingleUpdateDisplayParams& DisplayParams);

private:
	void UpdateLevelDisplay(const int32 NewLevel);
	void UpdateLineClearDisplay(const int32 NewLineClear, const int32 NewLineClearGoal);
	void UpdateTimeDisplay(const float NewTime);

	void OnUpdateTime();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlockBase> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlockBase> LineClearText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlockBase> TimeText;

	FTimerHandle UpdateTimeTimerHandle;
};
