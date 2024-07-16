// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "GoalSystemFactory.h"

#include "TetrisGameModeBase.generated.h"

class ATetrisPlayManager;
class ATetrisPlayerController;
class ATetrisPlayerStateBase;
class IGoalSystemInterface;
class UUserWidget;

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
class MULTIPLAYERTETRIS_API ATetrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATetrisGameModeBase();

	/**
	* Here, speed means the seconds it takes to move one line.
	*/
	float GetCurrentLevelNormalFallSpeed() const;
	ATetrisPlayManager* GetTetrisPlayManager() const { return TetrisPlayManager; }
	ATetrisPlayerController* GetTetrisPlayerController() const { return TetrisPlayerController; }

	void UpdateGamePlay(const FTetrisGamePlayInfo& UpdateInfo);

	virtual void PostLogin(APlayerController* const NewPlayer) override;

	/** statics methods */
	static float GetSoftDropSpeed(const float NormalFallSpeed) { return NormalFallSpeed / SoftDropMultiplier; }

protected:
	virtual void BeginPlay() override;

	void SetGoalSystemType(const EGoalSystemType NewGoalSystemType) { GoalSystemType = NewGoalSystemType; }

private:
	void Initialize();
	void StartGamePlay();

	static float CalculateNormalFallSpeed(const int32 GameLevel);

	//void ShowWidget(TSubclassOf<UUserWidget> WidgetClass);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Tetrimino")
	bool bNormalFallOff = false;

	// Queue
	static constexpr int32 NextQueueSize = 5;
	static constexpr int32 HoldQueueSize = 1;
	static constexpr int32 DefaultGameLevel = 1;
	static constexpr int32 DefaultScore = 0;
	static constexpr int32 DefaultLineCount = 0;

private:
	static constexpr int32 PlayerIndex = 0;
	static constexpr float SoftDropMultiplier = 20.0f;

protected:
	/** primitive */
	UPROPERTY(EditDefaultsOnly)
	EGoalSystemType GoalSystemType;

	/** Classes */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATetrisPlayManager> TetrisPlayManagerClass;

	/** Objects */
	UPROPERTY()
	TObjectPtr<ATetrisPlayManager> TetrisPlayManager;

	UPROPERTY()
	TObjectPtr<ATetrisPlayerController> TetrisPlayerController;

	UPROPERTY()
	TObjectPtr<ATetrisPlayerStateBase> TetrisPlayerState;

	// Declare Interface variable
	UPROPERTY()
	TScriptInterface<IGoalSystemInterface> GoalSystem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDSingleClass = nullptr;

	//UPROPERTY()
	//// Widgets
	//TObjectPtr<UUserWidget> CurrentWidget;
};
