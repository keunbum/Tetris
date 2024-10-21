// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisHudBase.h"
#include "TetrisHudIngameBase.generated.h"

class ATetrisGameModeIngameBase;
class ATetrisPlayerState;
class UHUDSingle;
class UTetrisWidgetHudIngame;

/**
 * 
 */
UCLASS(Abstract)
class TETRIS_API ATetrisHudIngameBase : public ATetrisHudBase
{
	GENERATED_BODY()

public:
	/** AActor */
	virtual void BeginPlay() override;
	/** ~AActor */

	/** ATetirsHudIngameBase */
	void Update();
	/** ~ATetirsHudIngameBase */

public:
	static constexpr float TimeDisplayUpdateInterval = 1.0f;

private:
	void OnUpdateTime();

protected:
	UPROPERTY()
	TObjectPtr<ATetrisGameModeIngameBase> TetrisGameMode;

	UPROPERTY()
	TObjectPtr<ATetrisPlayerState> TetrisPlayerState;

	UPROPERTY()
	TObjectPtr<UTetrisWidgetHudIngame> HudWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<UTetrisWidgetHudIngame> HudWidgetClass;
};
