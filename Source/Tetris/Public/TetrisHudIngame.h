// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisHudBase.h"
#include "TetrisHudIngame.generated.h"

class ATetrisGameModeIngame;
class ATetrisPlayerState;
class UTetrisWidgetHudIngame;

/**
 * 
 */
UCLASS()
class TETRIS_API ATetrisHudIngame : public ATetrisHudBase
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
	TObjectPtr<ATetrisGameModeIngame> TetrisGameMode;

	UPROPERTY()
	TObjectPtr<ATetrisPlayerState> TetrisPlayerState;

	UPROPERTY()
	TObjectPtr<UTetrisWidgetHudIngame> HudWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<UTetrisWidgetHudIngame> HudWidgetClass;
};
