// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TetriminoBase.h"
#include "TetrisInGameGameMode.h"

#include "TetrisPlayManager.generated.h"

class ATetrisInGameGameMode;
class ATetrimino;
class AGhostPiece;
class ABoard;
class ATetriminoQueue;
class UTetriminoGenerator;

UENUM()
enum class ELockDownOption : uint8
{
	ExtendedPlacement,
	InfinitePlacement,
	Classic,
};

UENUM()
enum class EPhase : uint16
{
	None,
	Generation,
	Falling,
	Lock,
	Pattern,
	Iterate,
	Animate,
	Elimate,
	Completion,
};

/**
 * @class ATetrisPlayManager
 * @brief The ATetrisPlayManager class is responsible for managing the gameplay logic of the Tetris game.
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisPlayManager : public AActor
{
	GENERATED_BODY()
	
public:
	ATetrisPlayManager();

	void Initialize();

	/** Basic Member Variable Accessors */
	void SetPhaseWithDelay(const EPhase NewPhase, const float Delay);
	const FVector2D& GetTetriminoMovementDirection() const { return TetriminoMovementDirection; }
	float GetNormalFallSpeed() const { return NormalFallSpeed; }
	void SetNormalFallSpeed(const float NewNormalFallSpeed) { NormalFallSpeed = NewNormalFallSpeed; }

	/** Event Handlers */
	void StartMovement(const FVector2D& InMovementDirection);
	void EndMovement();
	void StartSoftDrop();
	void EndSoftDrop();
	void DoHardDrop();
	void DoRotation(const ETetriminoRotationDirection RotationDirection);
	void HoldTetriminoInPlay();

private:
	/**  Initialization */
	void InitializeNextQueue();
	void InitializeHoldQueue();

	/** Phase Flow */
	void RunGenerationPhase();
	void RunFallingPhase();
	void RunLockPhase();
	void RunPatternPhase();
	void RunIteratePhase();
	void RunAnimatePhase();
	void RunEliminatePhase();
	void RunCompletionPhase();

	/** User Input */
	void MoveTetriminoTo(const FVector2D& Direction);
	void MoveTetrimino();
	void MoveTetriminoDown();
	void MoveTetriminoInPlayToFinalFallingLocation();

	void HardDrop();

	bool IsHoldingTetriminoInPlayAvailable() const;

	void RunSuperRotationSystem(const ETetriminoRotationDirection RotationDirection);

	/** Main Logic */
	void LockDown();
	void ForcedLockDown();

	/** Sub Logic */
	void CheckLineClearPattern(TArray<int32>& OutHitList);

	/** Timers */
	void SetAutoRepeatMovementTimer();
	void SetSoftDropTimer();
	void SetHardDropTimer();
	void SetNormalFallTimer();

	void ClearTimer(FTimerHandle& InOutTimerHandle);
	void ClearTimers(const TArray<FTimerHandle*>& TimerHandles);
	void ClearTetriminoInPlayLogicTimers();

	/** Basic Member Variable Accessors */
	void SetPhase(const EPhase NewPhase) { Phase = NewPhase; }
	void SetIsTetriminoInPlayManipulable(const bool bInIsTetriminoInPlayManipulable) { bIsTetriminoInPlayManipulable = bInIsTetriminoInPlayManipulable; }
	bool GetIsTetriminoInPlayManipulable() const { return bIsTetriminoInPlayManipulable; }
	void SetTetriminoMovementDirection(const FVector2D& NewTetriminoMovementDirection) { TetriminoMovementDirection = NewTetriminoMovementDirection; }
	void SetTetriminoInPlay(ATetrimino* const InTetriminoInPlay);

	/** Tetrimino Generation */
	ATetrimino* PopTetriminoFromNextQueue();
	void SpawnAndPushTetriminoToNextQueue();
	ATetrimino* SpawnNextTetrimino() const;

	/** Effect */
	void PlayLockDownEffect(const TArray<UMino*>& MinoArray);
	
private:
	/** Normal Fall */
	static constexpr bool bIsNormalFallTimerLoop = true;
	static constexpr float NormalFallTimerInitialDelay = 0.0f;

	/** Auto Repeat Movement */
	static constexpr bool bIsAutoRepeatMovementLoop = true;
	static constexpr float AutoRepeatMovementInitialDelay = 0.3f;
	static constexpr float AutoRepeatMovementInterval = 0.05f; // Adjust this value as needed

	/** Soft Drop */
	static constexpr bool bSoftDropTimerLoop = true;
	static constexpr float SoftDropTimerInitialDelay = 0.0f;

	/** Hard Drop */
	static constexpr bool bIsHardDropTimerLoop = false;
	static constexpr float HardDropTimerInitialDelay = 0.0001f;
	static constexpr float HardDropLockDownDelay = 0.0f;

	/** Phase Transition */
	static constexpr bool bIsPhaseTransitionTimerLoop = false;
	static constexpr float LockPhaseTransitionInitialDelayOfNormalFallOrSoftDrop = 0.5f;
	static constexpr float GenerationPhaseTransitionInitialDelay = 0.2f;

private:
	UPROPERTY(VisibleAnywhere)
	EPhase Phase;

	UPROPERTY()
	ELockDownOption LockDownOption;

	UPROPERTY(VisibleAnywhere)
	bool bIsTetriminoInPlayManipulable;

	UPROPERTY(EditDefaultsOnly)
	bool bIsGhostPieceOn;

	UPROPERTY()
	bool bIsTetriminoInPlayLockedDownFromLastHold;

	UPROPERTY(VisibleAnywhere)
	float NormalFallSpeed;

	UPROPERTY()
	TObjectPtr<ATetrisInGameGameMode> GameMode;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATetrimino> TetriminoClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetrimino> TetriminoInPlay;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGhostPiece> GhostPieceClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AGhostPiece> GhostPiece;

	UPROPERTY()
	TObjectPtr<ABoard> Board;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetriminoQueue> NextQueue;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetriminoQueue> HoldQueue;

	UPROPERTY()
	TObjectPtr<UTetriminoGenerator> TetriminoGenerator;

	UPROPERTY(VisibleAnywhere)
	FVector2D TetriminoMovementDirection;

	UPROPERTY(VisibleInstanceOnly)
	FTetrisGamePlayInfo GamePlayInfo;

	UPROPERTY(EditDefaultsOnly)
	ETetriminoShape TestSpawnShape = ETetriminoShape::None;

	/** TetriminoInPlay Logic Timers */
	FTimerHandle AutoRepeatMovementTimerHandle;
	FTimerHandle NormalFallTimerHandle;
	FTimerHandle SoftDropTimerHandle;
	FTimerHandle HardDropTimerHandle;

	/** Phase Timers */
	FTimerHandle PhaseChangeTimerHandle;
};
