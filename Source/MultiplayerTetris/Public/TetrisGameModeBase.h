// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "TetrisGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATetrisGameModeBase();

protected:
	virtual void BeginPlay() override;

private:
	void Initialize();

public:
	int32 GetGameLevel() const;
	void SetGameLevel(const int32 NewGameLevel);

	float GetFallSpeed() const;

private:
	static float CalculateFallSpeed(const int32 Level);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Tetromino")
	bool bTetrominoFallOff = false;

private:
	static constexpr int32 PlayerIndex = 0;
	static constexpr int32 DefaultGameLevel = 1;

	int32 GameLevel;
};
