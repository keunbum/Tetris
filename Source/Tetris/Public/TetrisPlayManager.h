// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TetriminoBase.h"
#include "TetrisInGameGameMode.h"
#include "Board.h"

#include "TetrisPlayManager.generated.h"

class ATetrisIngameGameMode;
class ATetrimino;
class AGhostPiece;
class ATetriminoQueue;
class UTetriminoGenerator;

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

struct FExtendedPlacement
{
	FExtendedPlacement()
		: TimerResetCount(MaxTimerResetCount)
		, LowestRow(0)
	{
	}

	void Init(const int32 CurrentRow)
	{
		LowestRow = CurrentRow;
		TimerResetCount = MaxTimerResetCount;
	}

	// The number of actions remaining before the forced lock down is reached.
	// The Actions: Left/Right Movement or Rotation
	int32 TimerResetCount;
	int32 LowestRow;

	static constexpr int32 MaxTimerResetCount = 15;
};

/**
 * @class ATetrisPlayManager
 * @brief The ATetrisPlayManager class is responsible for managing the gameplay logic of the Tetris game.
 */
UCLASS()
class TETRIS_API ATetrisPlayManager : public AActor
{
	GENERATED_BODY()
	
public:
	ATetrisPlayManager();

	void Initialize();

	/** Basic Member Variable Accessors */
	const FVector2D& GetTetriminoMovementDirection() const { return AutoRepeatMovementDirection; }
	float GetNormalFallSpeed() const { return NormalFallSpeed; }
	void SetNormalFallSpeed(const float NewNormalFallSpeed) { NormalFallSpeed = NewNormalFallSpeed; }

	/** Logic */
	void EnterPhase(const EPhase NewPhase);
	void EnterPhaseWithDelay(const EPhase NewPhase, const float Delay);

	/** Event Handlers */
	void StartAutoRepeatMovement(const FVector2D& InMovementDirection);
	void EndAutoRepeatMovement();
	void StartSoftDrop();
	void EndSoftDrop();
	void DoHardDrop();
	void DoRotation(const ETetriminoRotationDirection RotationDirection);
	void HoldTetriminoInPlay();

	/** static */
	static FName GetPhaseName(const EPhase Phase);
	static bool IsAutoRepeatMovement(const FVector2D& Direction) { return (Direction == ATetriminoBase::MoveDirectionLeft || Direction == ATetriminoBase::MoveDirectionRight); }

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

	/** Logics */
	// Move
	void MoveTetriminoTo(const FVector2D& Direction);
	void MoveTetrimino();
	void MoveTetriminoDown();
	void MoveTetriminoToInternal(const FVector2D& Direction);
	void MoveTetriminoInPlayToFinalFallingLocation();
	// LockDown
	void LockDown();
	void ForceLockDown();
	void HardDrop();
	void RunLockDownSystem();
	// Rotation
	void RunSuperRotationSystem(const ETetriminoRotationDirection RotationDirection);
	// CheckLineClear
	void CheckLineClearPattern(TArray<int32>& OutHitList);
	// Eliminate
	void RemoveTetriminoInPlay();
	// Etc
	bool IsHoldingTetriminoInPlayAvailable() const;
	bool IsSoftDropOrNormalFall(const FVector2D& Direction) const { return Direction == ATetriminoBase::MoveDirectionDown; }
	bool IsTetriminoInPlayOnSurface() const;

	/** Timers */
	void SetAutoRepeatMovementTimer();
	void SetSoftDropTimer();
	void SetNormalFallTimer();
	void SetLockDownTimer();

	void ClearTimer(FTimerHandle& InOutTimerHandle);
	void ClearTimerWithPrefix(const FString& Prefix, FTimerHandle& InOutTimerHandle);
	void ClearTimers(const TArray<FTimerHandle*>& TimerHandles);
	void ClearAllTimers();

	bool IsTimerActive(const FTimerHandle& TimerHandle) const;

	/** Basic Member Variable Accessors */
	void SetAutoRepeatMovementDirection(const FVector2D& NewTMovementDirection) { AutoRepeatMovementDirection = NewTMovementDirection; }
	void SetTetriminoInPlay(ATetrimino* const InTetriminoInPlay);

	/** Tetrimino Generation */
	ATetrimino* PopTetriminoFromNextQueue();
	void SpawnAndPushTetriminoToNextQueue();
	ATetrimino* SpawnNextTetrimino() const;

	/** Effect */
	void PlayLockDownEffect(const TArray<UMino*>& MinoArray);
	void PlaySoundCue(USoundCue* const SoundCue) const;
	
private:
	/** Normal Fall */
	static constexpr bool bIsNormalFallTimerLoop = true;
	static constexpr float NormalFallTimerInitialDelay = -1.0f;

	/** Auto Repeat Movement */
	static constexpr bool bIsAutoRepeatMovementLoop = true;
	static constexpr float AutoRepeatMovementInitialDelay = 0.3f;
	static constexpr float AutoRepeatMovementInterval = 0.05f; // Adjust this value as needed

	/** Soft Drop */
	static constexpr bool bSoftDropTimerLoop = true;
	static constexpr float SoftDropTimerInitialDelay = 0.0f;

	/** Lock Down */
	static constexpr bool bIsLockDownTimerLoop = false;
	static constexpr float LockDownTimerInitialDelay = 0.5f;

	/** Phase Change */
	static constexpr bool bIsPhaseChangeTimerLoop = false;
	static constexpr float GenerationPhaseChangeInitialDelay = 0.2f;

private:
	UPROPERTY(VisibleAnywhere)
	EPhase Phase;

	UPROPERTY(VisibleAnywhere)
	bool bIsTetriminoInPlayManipulable;

	UPROPERTY(EditDefaultsOnly)
	bool bIsGhostPieceOn;

	UPROPERTY()
	bool bHasLockDownFromLastHold;

	UPROPERTY(VisibleAnywhere)
	float NormalFallSpeed;

	UPROPERTY()
	TObjectPtr<ATetrisIngameGameMode> GameMode;

	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<ATetrimino> TetriminoClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetrimino> TetriminoInPlay;

	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<ABoard> BoardClass;

	UPROPERTY()
	TObjectPtr<ABoard> Board;

	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<AGhostPiece> GhostPieceClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AGhostPiece> GhostPiece;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetriminoQueue> NextQueue;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetriminoQueue> HoldQueue;

	UPROPERTY()
	TObjectPtr<UTetriminoGenerator> TetriminoGenerator;

	UPROPERTY(VisibleAnywhere)
	FVector2D AutoRepeatMovementDirection;

	UPROPERTY(VisibleInstanceOnly)
	FTetrisGamePlayInfo GamePlayInfo;

	/** Placement */
	FExtendedPlacement ExtendedPlacement;

	/** Test */
	UPROPERTY(EditDefaultsOnly, Category = "Test")
	ETetriminoShape TestSpawnShape = ETetriminoShape::None;

	/** Audio */
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundCue> MoveSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundCue> RotateSoundCue;

	/** Logic Timers */
	FTimerHandle AutoRepeatMovementTimerHandle;
	FTimerHandle NormalFallTimerHandle;
	FTimerHandle SoftDropTimerHandle;
	FTimerHandle LockDownTimerHandle;
};
