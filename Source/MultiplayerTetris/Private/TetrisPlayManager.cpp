// Copyright KeunBeom Ryu. All Rights Reserved.

#include "TetrisPlayManager.h"

#include "Board.h"
#include "TetrisGameModeBase.h"
#include "Tetrimino.h"
#include "TetrisPlayerController.h"
#include "TetriminoGenerator.h"

ATetrisPlayManager::ATetrisPlayManager()
	: Phase(EPhase::None)
	, LockDownOption(ELockDownOption::ExtendedPlacement)
	, bIsGhostPieceOn(true)
	, NormalFallSpeed(-1.0f)
	, TetriminoClass(ATetrimino::StaticClass())
	, TetriminoInPlay(nullptr)
	, TetriminoGenerator(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATetrisPlayManager::StartGenerationPhase()
{
	UE_LOG(LogTemp, Display, TEXT("Start Generation Phase."));

	SetPhase(EPhase::Generation);
	ATetrimino* const NewTetrimino = GetNextTetrimino();
	check(NewTetrimino != nullptr);
	ChangeTetrimino(NewTetrimino);

	StartFallingPhase();
}

void ATetrisPlayManager::StartFallingPhase()
{
	UE_LOG(LogTemp, Display, TEXT("Start Falling Phase."));

	SetPhase(EPhase::Falling);

	SetNormalFallTimer();

	ATetrisPlayerController* const PlayerController = GameMode->GetTetrisPlayerController();
	check(PlayerController != nullptr);
	if (PlayerController->IsSoftDropKeyPressed())
	{
		StartSoftDrop();
	}
}

void ATetrisPlayManager::StartMovement(const FVector2D& InMovementDirection)
{
	if (!IsTetriminoManipulable())
	{
		UE_LOG(LogTemp, Display, TEXT("Tetrimino is not manipulable."));
		return;
	}

	check(TetriminoInPlay != nullptr);

	SetMovementDirection(InMovementDirection);
	MoveTetriminoToCurrentDirection();
	SetAutoRepeatMovement();
}

void ATetrisPlayManager::EndMovement()
{
	if (TetriminoInPlay)
	{
		ClearTimer(AutoRepeatMovementTimerHandle);
		SetMovementDirection(FVector2D::ZeroVector);
	}
}

void ATetrisPlayManager::StartSoftDrop()
{
	if (!IsTetriminoManipulable())
	{
		UE_LOG(LogTemp, Display, TEXT("Tetrimino is not manipulable."));
		return;
	}

	check(TetriminoInPlay != nullptr);

	// NormalFall 일시 중지
	ClearTimer(NormalFallTimerHandle);

	SetSoftDropTimer();
}

void ATetrisPlayManager::EndSoftDrop()
{
	if (TetriminoInPlay)
	{
		ClearTimer(SoftDropTimerHandle);

		// NormalFall 재개
		SetNormalFallTimer();
	}
}

void ATetrisPlayManager::DoHardDrop()
{
	// TODO: 하드 드롭 로직 추가
}

void ATetrisPlayManager::DoRotation(const ETetriminoRotationDirection RotationDirection)
{
	if (!IsTetriminoManipulable())
	{
		UE_LOG(LogTemp, Display, TEXT("Tetrimino is not manipulable."));
		return;
	}

	RunSuperRotationSystem(RotationDirection);
}

void ATetrisPlayManager::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ATetrisPlayManager::Initialize()
{
	UWorld* const World = GetWorld();
	check(World != nullptr);
	GameMode = World->GetAuthGameMode<ATetrisGameModeBase>();

	SetNormalFallSpeed(GameMode->GetNormalFallSpeed());
	SetMovementDirection(FVector2D::ZeroVector);

	Board = World->SpawnActor<ABoard>();
	check(Board != nullptr);

	TetriminoGenerator = NewObject<UTetriminoGenerator>(this);
	check(TetriminoGenerator != nullptr);

	UMino::ClearMaterialCache();
}

void ATetrisPlayManager::MoveTetriminoTo(const FVector2D& Direction)
{
	if (!TetriminoInPlay)
	{
		UE_LOG(LogTemp, Display, TEXT("TetriminoInPlay is nullptr."));
		return;
	}

	const FIntPoint MovementIntVector2D = GetMovementIntVector2D(Direction);
	const bool bIsSoftDropOrNormalFall = (Direction == ATetrimino::MoveDirectionDown);
	const bool bIsMovementPossible = Board->IsMovementPossible(TetriminoInPlay, MovementIntVector2D);
	if (bIsMovementPossible)
	{
		TetriminoInPlay->MoveBy(MovementIntVector2D);

		const bool bIsOnSurface = !Board->IsMovementPossible(TetriminoInPlay, MovementIntVector2D);
		const bool bIsLockPhaseReached = bIsSoftDropOrNormalFall && bIsOnSurface;
		if (bIsLockPhaseReached)
		{
			UE_LOG(LogTemp, Display, TEXT("LockDown Timer is set."));
			SetLockDownTimer();
		}
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("Movement is impossible."));
	}
}

void ATetrisPlayManager::MoveTetriminoToCurrentDirection()
{
	MoveTetriminoTo(CurrentMovementDirection);
}

void ATetrisPlayManager::MoveTetriminoDown()
{
	MoveTetriminoTo(ATetrimino::MoveDirectionDown);
}

void ATetrisPlayManager::RunSuperRotationSystem(const ETetriminoRotationDirection RotationDirection)
{
	if (!TetriminoInPlay)
	{
		return;
	}

	const TArray<FIntPoint>& RotationPointOffsets = TetriminoInPlay->GetSRSRotationPointOffsets(RotationDirection);
	for (int32 PointIndex = 0; PointIndex < RotationPointOffsets.Num(); ++PointIndex)
	{
		const FIntPoint& RotationPointOffset = RotationPointOffsets[PointIndex];
		if (Board->IsRotationPossible(TetriminoInPlay, RotationDirection, RotationPointOffset))
		{
			TetriminoInPlay->RotateTo(RotationDirection);
			TetriminoInPlay->MoveBy(RotationPointOffset);
			//UE_LOG(LogTemp, Display, TEXT("%Rotation with Point%d."), PointIndex + 1);
			return;
		}
	}
	//UE_LOG(LogTemp, Display, TEXT("All of Rotation Points failed."));
}

void ATetrisPlayManager::LockDown()
{
	UE_LOG(LogTemp, Display, TEXT("Lock Down."));

	SetPhase(EPhase::LockDown);

	check(TetriminoInPlay != nullptr);

	PlayLockDownEffect(TetriminoInPlay->GetMinoArray());

	// Disable all user input timers
	ClearUserInputTimers();

	// Transfer of TetriminoInPlay's Minos to Board
	TetriminoInPlay->DetachMinos();
	Board->AddMinos(TetriminoInPlay);

	// Remove TetriminoInPlay
	TetriminoInPlay->Destroy();
	TetriminoInPlay = nullptr;

	// Switch to Generation Phase.
	GetWorldTimerManager().SetTimer(GenerationPhaseTimerHandle, this, &ATetrisPlayManager::StartGenerationPhase, GenerationPhaseInitialDelay, bIsGenerationPhaseTimerLoop);
}

void ATetrisPlayManager::ForcedLockDown()
{
	ClearTimer(LockDownTimerHandle);
	LockDown();
}

void ATetrisPlayManager::SetAutoRepeatMovement()
{
	GetWorldTimerManager().SetTimer(AutoRepeatMovementTimerHandle, this, &ATetrisPlayManager::MoveTetriminoToCurrentDirection, AutoRepeatMovementInterval, bIsAutoRepeatMovementLoop, AutoRepeatMovementInitialDelay);
}

void ATetrisPlayManager::SetSoftDropTimer()
{
	GetWorldTimerManager().SetTimer(SoftDropTimerHandle, this, &ATetrisPlayManager::MoveTetriminoDown, GameMode->GetSoftDropSpeed(), bSoftDropTimerLoop, SoftDropTimerInitialDelay);
}

void ATetrisPlayManager::SetNormalFallTimer()
{
	if (GameMode && !GameMode->bNormalFallOff)
	{
		UE_LOG(LogTemp, Display, TEXT("Normal Fall Timer is set."));
		GetWorldTimerManager().SetTimer(NormalFallTimerHandle, this, &ATetrisPlayManager::MoveTetriminoDown, NormalFallSpeed, bIsNormalFallTimerLoop, NormalFallTimerInitialDelay);
	}
}

void ATetrisPlayManager::SetLockDownTimer()
{
	GetWorldTimerManager().SetTimer(LockDownTimerHandle, this, &ATetrisPlayManager::LockDown, LockDownTimerInitialDelay, bIsLockDownTimerLoop);
}

void ATetrisPlayManager::ClearTimer(FTimerHandle& InOutTimerHandle)
{
	GetWorldTimerManager().ClearTimer(InOutTimerHandle);
}

void ATetrisPlayManager::ClearTimers(const TArray<FTimerHandle*>& TimerHandles)
{
	for (FTimerHandle* const TimerHandle : TimerHandles)
	{
		if (TimerHandle)
		{
			ClearTimer(*TimerHandle);
		}
	}
}

void ATetrisPlayManager::ClearUserInputTimers()
{
	const TArray<FTimerHandle*> UserInputTimerHandles =
	{
		&AutoRepeatMovementTimerHandle,
		&SoftDropTimerHandle,
		&NormalFallTimerHandle
	};
	ClearTimers(UserInputTimerHandles);
}

ATetrimino* ATetrisPlayManager::GetNextTetrimino() const
{
#define TETRIMINO_SPAWN_RANDOM 1

#if TETRIMINO_SPAWN_RANDOM == 1
	return TetriminoGenerator->SpawnTetriminoByBagSystem(TetriminoClass);
#else
	return TetriminoGenerator->SpawnTetriminoByShape(TetriminoClass, TestSpawnShape);
#endif
}

void ATetrisPlayManager::ChangeTetrimino(ATetrimino* const NewTetrimino)
{
	check(NewTetrimino != nullptr);
	NewTetrimino->AttachToBoard(Board);
	SetTetriminoInPlay(NewTetrimino);
}

void ATetrisPlayManager::PlayLockDownEffect(const TArray<UMino*>& GetMinoArray)
{
}

FIntPoint ATetrisPlayManager::GetMovementIntVector2D(const FVector2D& Direction)
{
	static constexpr float OneSpace = 1.0f;
	const FVector2D MovementVector2D = OneSpace * Direction;
	return FIntPoint(static_cast<int32>(MovementVector2D.X), static_cast<int32>(MovementVector2D.Y));
}
