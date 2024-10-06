// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "TetrisInGameGameMode.h"

#include "TetrisPlayerStateBase.generated.h"

class IGoalSystemInterface;
struct FHUDSingleUpdateDisplayParams;

/**
 * 
 */
UCLASS()
class TETRIS_API ATetrisPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	ATetrisPlayerStateBase();

	void Initialize(const IGoalSystemInterface* GoalSystem);

	// Score (현재 점수. 부모 클래스의 멤버 변수 그대로 사용)
	void AddScore(const int32 Value) { SetScore(GetScore() + Value); }

	// GameLevel
	int32 GetGameLevel() const { return GameLevel; }
	void SetGameLevel(const int32 NewGameLevel) { GameLevel = NewGameLevel; }
	void AddGameLevel(const int32 Value) { SetGameLevel(GetGameLevel() + Value); }
	void LevelUp(const IGoalSystemInterface* GoalSystem);

	// LineClearCount (현재 레벨에서 클리어한 라인 수)
	int32 GetLineClearCount() const { return LineClearCount; }
	void SetLineClearCount(const int32 NewLineClearCount) { LineClearCount = NewLineClearCount; }
	void AddLineClearCount(const int32 Value) { SetLineClearCount(GetLineClearCount() + Value); }

	// TotalLineClearCount
	int32 GetTotalLineClearCount() const { return TotalLineClearCount; }
	void SetTotalLineClearCount(const int32 NewTotalLineClearCount) { TotalLineClearCount = NewTotalLineClearCount; }
	void AddTotalLineClearCount(const int32 Value) { SetTotalLineClearCount(GetTotalLineClearCount() + Value); }

	// LineClearGoal (현재 레벨에서 목표로 하는 라인 클리어 수)
	int32 GetLineClearGoal() const { return LineClearGoal; }
	void SetLineClearGoal(const int32 NewLineClearGoal) { LineClearGoal = NewLineClearGoal; }
	void SubtractLineClearGoal(const int32 Value) { SetLineClearGoal(GetLineClearGoal() - Value); }

	FHUDSingleUpdateDisplayParams GetHUDSingleUpdateDisplayParams() const;

	void UpdateState(const FTetrisGamePlayInfo& PlayInfo);
	void DebugPrint() const;

private:
	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 GameLevel;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 LineClearCount;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 TotalLineClearCount;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 LineClearGoal;
};
