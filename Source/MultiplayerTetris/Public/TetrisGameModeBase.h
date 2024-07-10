// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "TetrisGameModeBase.generated.h"

class ATetrisPlayManager;
class ATetrisPlayerController;
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
	float GetNormalFallSpeed() const { return CalculateNormalFallSpeed(CurrentLevel); }
	float GetSoftDropSpeed() const { return GetNormalFallSpeed() / SoftDropMultiplier; }

	ATetrisPlayManager* GetTetrisPlayManager() const { return TetrisPlayManager; }
	ATetrisPlayerController* GetTetrisPlayerController() const { return TetrisPlayerController; }

protected:
	virtual void BeginPlay() override;

private:
	void Initialize();
	void StartGamePlay();

	static float CalculateNormalFallSpeed(const int32 Level);

	//void ShowWidget(TSubclassOf<UUserWidget> WidgetClass);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Tetrimino")
	bool bNormalFallOff = false;

	// Queue
	static constexpr int32 NextQueueSize = 5;
	static constexpr int32 HoldQueueSize = 1;

private:
	static constexpr int32 PlayerIndex = 0;
	static constexpr int32 DefaultGameLevel = 1;
	static constexpr float SoftDropMultiplier = 20.0f;

protected:
	UPROPERTY(VisibleAnywhere)
	int32 CurrentLevel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATetrisPlayManager> TetrisPlayManagerClass;

	UPROPERTY()
	TObjectPtr<ATetrisPlayManager> TetrisPlayManager;

	UPROPERTY()
	TObjectPtr<ATetrisPlayerController> TetrisPlayerController;

	//UPROPERTY()
	//// Widgets
	//TObjectPtr<UUserWidget> CurrentWidget;
};
