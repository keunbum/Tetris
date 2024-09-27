// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisGameModeBase.h"

#include "GoalSystemFactory.h"

#include "TetrisInGameGameMode.generated.h"

class ATetrisPlayManager;
class ATetrisPlayerControllerSingle;
class ATetrisPlayerStateBase;
class IGoalSystemInterface;
class UHUDSingle;

USTRUCT()
struct FTetrisGamePlayInfo
{
	GENERATED_BODY()

	TArray<int32> HitList; // List of rows to be cleared

	void Reset()
	{
		HitList.Empty();
	}
};

/**
 * 
 */
UCLASS()
class TETRIS_API ATetrisInGameGameMode : public ATetrisGameModeBase
{
	GENERATED_BODY()
	
public:
	ATetrisInGameGameMode();

	/** AGameModeBase */
	virtual void PostLogin(APlayerController* const NewPlayer) override;
	/** ~AGameModeBase */

	float GetElapsedTime() const;
	// Here, speed means the seconds it takes to move one line.
	float GetCurrentLevelNormalFallSpeed() const;
	ATetrisPlayManager* GetTetrisPlayManager() const { return TetrisPlayManager; }
	ATetrisPlayerControllerSingle* GetTetrisPlayerController() const { return TetrisPlayerController; }

	void UpdateGamePlay(const FTetrisGamePlayInfo& UpdateInfo);
	void RunGameOver();

	/** statics methods */
	static float GetSoftDropSpeed(const float NormalFallSpeed) { return NormalFallSpeed / SoftDropMultiplier; }

protected:
	/** AActor */
	virtual void BeginPlay() override;
	/** ~AActor */

	void SetGoalSystemType(const EGoalSystemType NewGoalSystemType) { GoalSystemType = NewGoalSystemType; }
	void LevelUp();

private:
	/** ATetrisGameModeBase */
	virtual void Initialize() override;
	virtual void SetInputMode() override;
	virtual void InitializeDefaultEffect() override;
	/** ~ATetrisGameModeBase */

	void StartGamePlay();

	/** static methods */
	static float CalculateNormalFallSpeed(const int32 GameLevel);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Tetrimino")
	bool bNormalFallOff = false;

	/** Queue */
	static constexpr int32 NextQueueSize = 5;
	static constexpr int32 HoldQueueSize = 1;

	/** Default Value */
	static constexpr int32 DefaultGameLevel = 1;
	static constexpr int32 DefaultScore = 0;
	static constexpr int32 DefaultLineCount = 0;

	// Level Name
	static const FName TetrisLevelName;

private:
	static constexpr float SoftDropMultiplier = 20.0f;

protected:
	/** primitive */
	UPROPERTY(EditDefaultsOnly)
	EGoalSystemType GoalSystemType;

	UPROPERTY(VisibleInstanceOnly)
	float GameStartTime; // seconds

	/** Classes */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATetrisPlayManager> TetrisPlayManagerClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHUDSingle> HUDWidgetClass = nullptr;

	/** Objects */
	UPROPERTY()
	TObjectPtr<ATetrisPlayManager> TetrisPlayManager;

	UPROPERTY()
	TObjectPtr<ATetrisPlayerControllerSingle> TetrisPlayerController;

	UPROPERTY()
	TObjectPtr<ATetrisPlayerStateBase> TetrisPlayerState;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<UHUDSingle> HUDWidget;

	/** Interfaces */
	UPROPERTY()
	TScriptInterface<IGoalSystemInterface> GoalSystem;
};
