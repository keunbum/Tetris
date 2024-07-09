// Copyright © 2024 Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TetriminoBase.h"

#include "TetrisPlayManager.generated.h"

class ATetrisGameModeBase;
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
enum class EPhase : uint8
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

	const FVector2D& GetTetriminoMovementDirection() const { return TetriminoMovementDirection; }
	void SetNormalFallSpeed(const float NewNormalFallSpeed) { NormalFallSpeed = NewNormalFallSpeed; }

	void StartGenerationPhase();

	// Event Handlers
	void StartMovement(const FVector2D& InMovementDirection);
	void EndMovement();
	void StartSoftDrop();
	void EndSoftDrop();
	void DoHardDrop();
	void DoRotation(const ETetriminoRotationDirection RotationDirection);

private:
	// Initialization
	void InitializeNextQueue();

	// Phase Flow
	void StartFallingPhase();
	void StartLockPhase(const float LockDownFirstDelay);

	// User Input
	void MoveTetriminoTo(const FVector2D& Direction);
	void MoveTetrimino();
	void MoveTetriminoDown();
	void HardDrop();
	// Move the Tetrimino to FinalFallingMatrixLocation.
	void MoveTetriminoToFinalFallingMatrixLocation();

	void RunSuperRotationSystem(const ETetriminoRotationDirection RotationDirection);

	void LockDown();
	void ForcedLockDown();

	void SetAutoRepeatMovementTimer();
	void SetSoftDropTimer();
	void SetHardDropTimer();
	void SetNormalFallTimer();
	void SetLockDownTimer(const float FirstDelay);

	void ClearTimer(FTimerHandle& InOutTimerHandle);
	void ClearTimers(const TArray<FTimerHandle*>& TimerHandles);
	void ClearUserInputTimers();

	// Tetrimino
	ATetrimino* PopTetriminoFromNextQueue();
	void SpawnAndPushTetriminoToNextQueue();
	ATetrimino* SpawnNextTetrimino() const;

	// Basic Member Variables
	void SetPhase(const EPhase NewPhase) { Phase = NewPhase; }
	void SetIsTetriminoInPlayManipulable(const bool bInIsTetriminoInPlayManipulable) { bIsTetriminoInPlayManipulable = bInIsTetriminoInPlayManipulable; }
	bool IsTetriminoInPlayManipulable() const { return bIsTetriminoInPlayManipulable; }
	void SetTetriminoMovementDirection(const FVector2D& NewTetriminoMovementDirection) { TetriminoMovementDirection = NewTetriminoMovementDirection; }
	void SetTetriminoInPlay(ATetrimino* const NewTetriminoInPlay);

	// Effect
	void PlayLockDownEffect(const TArray<UMino*>& MinoArray);
	
private:
	// Normal Fall
	static constexpr bool bIsNormalFallTimerLoop = true;
	static constexpr float NormalFallTimerInitialDelay = 0.0f;

	// Auto Repeat Movement
	static constexpr bool bIsAutoRepeatMovementLoop = true;
	static constexpr float AutoRepeatMovementInitialDelay = 0.3f;
	static constexpr float AutoRepeatMovementInterval = 0.05f; // Adjust this value as needed

	// Soft Drop
	static constexpr bool bSoftDropTimerLoop = true;
	static constexpr float SoftDropTimerInitialDelay = 0.0f;

	// Hard Drop
	static constexpr bool bIsHardDropTimerLoop = false;
	static constexpr float HardDropTimerInitialDelay = 0.0001f;
	static constexpr float LockDownDelayOfHardDrop = 0.0f;

	// LockDown
	static constexpr bool bIsLockDownTimerLoop = false;
	static constexpr float LockDownTimerInitialDelayOfNormalFallOrSoftDrop = 0.5f;

	// GenerationPhase
	static constexpr bool bIsGenerationPhaseTimerLoop = false;
	static constexpr float GenerationPhaseInitialDelay = 0.2f;

private:
	UPROPERTY(VisibleAnywhere)
	EPhase Phase;

	UPROPERTY()
	ELockDownOption LockDownOption;

	UPROPERTY(VisibleAnywhere)
	bool bIsTetriminoInPlayManipulable;

	UPROPERTY(EditDefaultsOnly)
	bool bIsGhostPieceOn;

	UPROPERTY(EditDefaultsOnly)
	bool bIsHoldQueueOn;

	UPROPERTY(VisibleAnywhere)
	float NormalFallSpeed;

	UPROPERTY()
	TObjectPtr<ATetrisGameModeBase> GameMode;

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
	FVector2D TetriminoMovementDirection;

	UPROPERTY()
	TObjectPtr<UTetriminoGenerator> TetriminoGenerator;

	UPROPERTY(EditDefaultsOnly)
	ETetriminoShape TestSpawnShape = ETetriminoShape::None;

	// User Input Timers
	FTimerHandle NormalFallTimerHandle;
	FTimerHandle SoftDropTimerHandle;
	FTimerHandle AutoRepeatMovementTimerHandle;
	FTimerHandle HardDropTimerHandle;

	// Game Logic Timers
	FTimerHandle LockDownTimerHandle;
	FTimerHandle GenerationPhaseTimerHandle;
};
