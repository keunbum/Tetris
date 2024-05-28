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

	/**
	* Here, speed means the seconds it takes to move one line.
	*/
	float GetFallSpeed() const;
	float GetSoftDropSpeed() const { return GetFallSpeed() / SoftDropMultiplier; }

private:
	static float CalculateFallSpeed(const int32 Level);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Tetromino")
	bool bNormalFallOff = false;

private:
	static constexpr int32 PlayerIndex = 0;
	static constexpr int32 DefaultGameLevel = 1;
	static constexpr float SoftDropMultiplier = 20.0f;

private:
	int32 GameLevel;
};
