// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "TetrisGameModeBase.generated.h"

class ATetrisMatrix;
class ATetrimino;
class ATetrisPlayManager;
class UUserWidget;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATetrisGameModeBase();

	int32 GetCurrentLevel() const { return CurrentLevel; }
	void LevelUp();

	/**
	* Here, speed means the seconds it takes to move one line.
	*/
	float GetFallSpeed() const { return CalculateFallSpeed(CurrentLevel); }
	float GetSoftDropSpeed() const { return GetFallSpeed() / SoftDropMultiplier; }

	ATetrisPlayManager* GetTetrisPlayManager() const { return TetrisPlayManager; }

protected:
	virtual void BeginPlay() override;

private:
	void Initialize();
	void StartGenerationPhase();
	void StartCompletionPhase();

	ATetrimino* SpawnNextTetrimino() const;

	static float CalculateFallSpeed(const int32 Level);

	//void ShowWidget(TSubclassOf<UUserWidget> WidgetClass);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Tetromino")
	bool bNormalFallOff = false;

private:
	static constexpr int32 PlayerIndex = 0;
	static constexpr int32 DefaultGameLevel = 1;
	static constexpr float SoftDropMultiplier = 20.0f;

protected:
	// Level
	int32 CurrentLevel;

	// Tetrimino
	UPROPERTY()
	TSubclassOf<ATetrimino> TetriminoClass;

	UPROPERTY()
	TObjectPtr<ATetrisPlayManager> TetrisPlayManager;

private:
	// Widgets
	TObjectPtr<UUserWidget> CurrentWidget;
};
