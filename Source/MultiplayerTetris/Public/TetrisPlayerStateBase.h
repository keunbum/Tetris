// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "TetrisGameModeBase.h"

#include "TetrisPlayerStateBase.generated.h"

class IGoalSystemInterface;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	ATetrisPlayerStateBase();

	void Initialize(const IGoalSystemInterface* GoalSystem);

	void AddScore(const int32 Value) { SetScore(GetScore() + Value); }

	int32 GetGameLevel() const { return GameLevel; }
	void SetGameLevel(const int32 NewGameLevel) { GameLevel = NewGameLevel; }
	void AddGameLevel(const int32 Value) { SetGameLevel(GetGameLevel() + Value); }
	void LevelUp(const IGoalSystemInterface* GoalSystem);

	int32 GetLineClearCount() const { return LineClearCount; }
	void SetLineClearCount(const int32 NewLineClearCount) { LineClearCount = NewLineClearCount; }
	void AddLineClearCount(const int32 Value) { SetLineClearCount(GetLineClearCount() + Value); }

	int32 GetTotalLineClearCount() const { return TotalLineClearCount; }
	void SetTotalLineClearCount(const int32 NewTotalLineClearCount) { TotalLineClearCount = NewTotalLineClearCount; }
	void AddTotalLineClearCount(const int32 Value) { SetTotalLineClearCount(GetTotalLineClearCount() + Value); }

	int32 GetGoalLineClear() const { return GoalLineClear; }
	void SetGoalLineClear(const int32 NewGoalLineClear) { GoalLineClear = NewGoalLineClear; }
	void SubtractGoalLineClear(const int32 Value) { SetGoalLineClear(GetGoalLineClear() - Value); }

	void UpdateState(const FTetrisGamePlayInfo& PlayInfo);

private:
	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 GameLevel;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 LineClearCount;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 TotalLineClearCount;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 GoalLineClear;
};
