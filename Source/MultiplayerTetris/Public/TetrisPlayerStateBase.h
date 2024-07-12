// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
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

	int32 GetScore() const { return Score; }
	void AddScore(const int32 AddedScore) { Score += AddedScore; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 GameLevel;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 Score;
};
