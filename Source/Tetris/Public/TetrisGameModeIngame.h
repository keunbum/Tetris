// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisGameModeBase.h"

#include "GoalSystemFactory.h"

#include "TetrisGameModeIngame.generated.h"

class ATetrisPlayManager;
class ATetrisPlayerControllerIngameSingle;
class ATetrisPlayerState;
class IGoalSystemInterface;

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
UCLASS(Abstract)
class TETRIS_API ATetrisGameModeIngame : public ATetrisGameModeBase
{
	GENERATED_BODY()
	
public:
	ATetrisGameModeIngame();

	/** AGameModeBase */
	virtual void PostLogin(APlayerController* const NewPlayer) override;
	/** ~AGameModeBase */

	float GetElapsedTime() const;
	// Here, speed means the seconds it takes to move one line.
	float GetCurrentLevelNormalFallSpeed() const;
	ATetrisPlayManager* GetTetrisPlayManager() const { return TetrisPlayManager; }
	ATetrisPlayerControllerIngameSingle* GetTetrisPlayerController() const { return TetrisPlayerController; }
	ATetrisPlayerState* GetTetrisPlayerState() const { return TetrisPlayerState; }

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
	virtual void InitializeEffect() override;
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
	static constexpr int32 DefaultLineClearCount = 0;

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
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<ATetrisPlayManager> TetrisPlayManagerClass = nullptr;

	/** Objects */
	UPROPERTY()
	TObjectPtr<ATetrisPlayManager> TetrisPlayManager;

	UPROPERTY()
	TObjectPtr<ATetrisPlayerControllerIngameSingle> TetrisPlayerController;

	UPROPERTY()
	TObjectPtr<ATetrisPlayerState> TetrisPlayerState;

	/** Interfaces */
	UPROPERTY()
	TScriptInterface<IGoalSystemInterface> GoalSystem;
};
