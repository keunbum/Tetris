// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "TetrisGameModeBase.h"

#include "TetrisPlayerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	ATetrisPlayerStateBase();

	int32 GetGameLevel() const { return GameLevel; }
	void SetGameLevel(const int32 NewGameLevel) { GameLevel = NewGameLevel; }

	void AddScore(const int32 AddedScore) { SetScore(GetScore() + AddedScore); }

	int32 GetLineClearCount() const { return LineClearCount; }
	void SetLineClearCount(const int32 NewLineClearCount) { LineClearCount = NewLineClearCount; }
	void AddLineClearCount(const int32 AddedLineClearCount) { SetLineClearCount(GetLineClearCount() + AddedLineClearCount); }

	void UpdateState(const FTetrisGamePlayInfo& UpdateInfo);

private:
	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 GameLevel;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 LineClearCount;
};
