// Copyright © 2024 Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayManager.h"

#include "Board.h"
#include "TetrisGameModeBase.h"
#include "Tetrimino.h"
#include "GhostPiece.h"
#include "TetrisPlayerController.h"
#include "TetriminoGenerator.h"
#include "TetriminoQueue.h"

ATetrisPlayManager::ATetrisPlayManager()
	: Phase(EPhase::None)
	, LockDownOption(ELockDownOption::ExtendedPlacement)
	, bIsTetriminoInPlayManipulable(false)
	, bIsGhostPieceOn(true)
	, NormalFallSpeed(-1.0f)
	, TetriminoClass(ATetrimino::StaticClass())
	, TetriminoInPlay(nullptr)
	, GhostPieceClass(AGhostPiece::StaticClass())
	, GhostPiece(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATetrisPlayManager::Initialize()
{
	UE_LOG(LogTemp, Display, TEXT("TetrisPlayManager is initialized."));

	// UMino
	UMino::ClearMaterialCache();

	// World
	UWorld* const World = GetWorld();
	check(World != nullptr);

	// GameMode
	GameMode = World->GetAuthGameMode<ATetrisGameModeBase>();

	// Set Basic members
	SetNormalFallSpeed(GameMode->GetNormalFallSpeed());
	SetTetriminoMovementDirection(FVector2D::ZeroVector);

	// Board
	Board = World->SpawnActor<ABoard>();
	check(Board != nullptr);
	Board->Initialize();

	// TetriminoGenerator
	TetriminoGenerator = NewObject<UTetriminoGenerator>(this);
	check(TetriminoGenerator != nullptr);

	// NextQueue
	NextQueue = World->SpawnActor<ATetriminoQueue>(ATetriminoQueue::StaticClass());
	check(NextQueue != nullptr);
	InitializeNextQueue();

	// GhostPiece
	GhostPiece = World->SpawnActor<AGhostPiece>(GhostPieceClass);
	check(GhostPiece != nullptr);
	GhostPiece->AttachToBoard(Board);
}

void ATetrisPlayManager::StartGenerationPhase()
{
	UE_LOG(LogTemp, Display, TEXT("Start Generation Phase."));

	SetPhase(EPhase::Generation);
	ATetrimino* const NewTetriminoInPlay = PopTetriminoFromNextQueue();

	SetTetriminoInPlay(NewTetriminoInPlay);

	// Set GhostPiece
	TetriminoInPlay->SetGhostPiece(GhostPiece);
	GhostPiece->SetRelativeLocationByMatrixLocation(Board->GetFinalFallingMatrixLocation(TetriminoInPlay));

	StartFallingPhase();
}

void ATetrisPlayManager::StartMovement(const FVector2D& InMovementDirection)
{
	if (!IsTetriminoInPlayManipulable())
	{
		UE_LOG(LogTemp, Display, TEXT("Tetrimino is not manipulable."));
		return;
	}

	check(TetriminoInPlay != nullptr);

	SetTetriminoMovementDirection(InMovementDirection);
	MoveTetrimino();
	SetAutoRepeatMovementTimer();
}

void ATetrisPlayManager::EndMovement()
{
	if (TetriminoInPlay)
	{
		ClearTimer(AutoRepeatMovementTimerHandle);
		SetTetriminoMovementDirection(FVector2D::ZeroVector);
	}
}

void ATetrisPlayManager::StartSoftDrop()
{
	if (!IsTetriminoInPlayManipulable())
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
	// 테트리스 가이드라인 2009에 나와 있는 방식 그대로 구현하기.
	// HardDrop에는 Auto-Repeat 없음.
	// TODO: 나중에 Hard Drop Trail 관련 이펙트도 있으면 금상첨화.
	if (!IsTetriminoInPlayManipulable())
	{
		UE_LOG(LogTemp, Display, TEXT("Tetrimino is not manipulable."));
		return;
	}
	SetHardDropTimer();
}

void ATetrisPlayManager::DoRotation(const ETetriminoRotationDirection RotationDirection)
{
	if (!IsTetriminoInPlayManipulable())
	{
		UE_LOG(LogTemp, Display, TEXT("Tetrimino is not manipulable."));
		return;
	}

	RunSuperRotationSystem(RotationDirection);
}

void ATetrisPlayManager::InitializeNextQueue()
{
	NextQueue->Initialize(GameMode->NextQueueSize, Board->GetNextQueueRoot());
	for (int32 Count = 0; Count < GameMode->NextQueueSize; ++Count)
	{
		SpawnAndPushTetriminoToNextQueue();
	}
	NextQueue->ReArrangeTetriminoLocations();
}

void ATetrisPlayManager::StartFallingPhase()
{
	UE_LOG(LogTemp, Display, TEXT("Start Falling Phase."));

	SetPhase(EPhase::Falling);
	SetIsTetriminoInPlayManipulable(true);

	SetNormalFallTimer();

	ATetrisPlayerController* const PlayerController = GameMode->GetTetrisPlayerController();
	check(PlayerController != nullptr);
	if (PlayerController->IsSoftDropKeyPressed())
	{
		StartSoftDrop();
	}
}

void ATetrisPlayManager::StartLockPhase(const float LockDownDelay)
{
	UE_LOG(LogTemp, Display, TEXT("Start Lock Phase."));

	SetPhase(EPhase::Lock);
	SetLockDownTimer(LockDownDelay);
}

void ATetrisPlayManager::MoveTetriminoTo(const FVector2D& Direction)
{
	if (!TetriminoInPlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayManager::MoveTetriminoTo(): TetriminoInPlay is nullptr."));
		return;
	}

	const FIntPoint MovementIntPoint = ATetriminoBase::GetMatrixMovementIntPointByDirection(Direction);
	const bool bIsMovementPossible = Board->IsMovementPossible(TetriminoInPlay, MovementIntPoint);
	if (bIsMovementPossible)
	{
		TetriminoInPlay->MoveBy(MovementIntPoint);

		const bool bIsSoftDropOrNormalFall = (Direction == ATetrimino::MoveDirectionDown);
		const bool bIsOnSurface = !Board->IsMovementPossible(TetriminoInPlay, MovementIntPoint);
		const bool bIsLockPhaseReached = bIsSoftDropOrNormalFall && bIsOnSurface;
		if (bIsLockPhaseReached)
		{
			StartLockPhase(LockDownTimerInitialDelayForNormalFallOrSoftDrop);
		}
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("Movement is impossible."));
	}
}

void ATetrisPlayManager::MoveTetrimino()
{
	MoveTetriminoTo(TetriminoMovementDirection);
}

void ATetrisPlayManager::MoveTetriminoDown()
{
	MoveTetriminoTo(ATetriminoBase::MoveDirectionDown);
}

void ATetrisPlayManager::HardDrop()
{
	check(IsTetriminoInPlayManipulable());
	MoveTetriminoToFinalFallingMatrixLocation();
	ForcedLockDown();
}

void ATetrisPlayManager::MoveTetriminoToFinalFallingMatrixLocation()
{
	const FIntPoint FinalFallingMatrixLocation = GhostPiece->GetMatrixLocation();
	TetriminoInPlay->SetRelativeLocationByMatrixLocation(FinalFallingMatrixLocation);
}

void ATetrisPlayManager::RunSuperRotationSystem(const ETetriminoRotationDirection RotationDirection)
{
	check(TetriminoInPlay != nullptr);

	const TArray<FIntPoint>& SRSRotationPointOffsets = TetriminoInPlay->GetSRSRotationPointOffsets(RotationDirection);
	for (const FIntPoint& SRSRotationPointOffset : SRSRotationPointOffsets)
	{
		const bool bIsRotationPossible = Board->IsRotationPossible(TetriminoInPlay, RotationDirection, SRSRotationPointOffset);
		if (bIsRotationPossible)
		{
			TetriminoInPlay->RotateTo(RotationDirection);
			TetriminoInPlay->MoveBy(SRSRotationPointOffset);
			//UE_LOG(LogTemp, Display, TEXT("%Rotation with Point%d."), PointIndex + 1);
			return;
		}
	}
	//UE_LOG(LogTemp, Display, TEXT("All of Rotation Points failed."));
}

void ATetrisPlayManager::LockDown()
{
	UE_LOG(LogTemp, Display, TEXT("Lock Down."));

	check(TetriminoInPlay != nullptr);

	SetIsTetriminoInPlayManipulable(false);

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
	// TODO: 아 이건 좀.. 그.. 아니 만약에 LockDownTimerHandle 남은 시간이 HardDropTimerInitialDelay보다 짧을 수도 있잖아?
	// 가이드라인에 정확히는 안나와 있는데.. HardDrop의 강제성이 더 높아야 할 것 같은데..
	// 물론 이건 추후에 다른 기능 구현하다보면 구체화될 수도 있는 거니, 지금은 이 정도로 넘어 가는 걸로.
	// 버그는 아니잖아.
	ClearTimer(LockDownTimerHandle);

	// GenerationPhaseTimerHandle should be finished.
	check(!GetWorldTimerManager().IsTimerActive(GenerationPhaseTimerHandle));

	LockDown();
}

void ATetrisPlayManager::SetAutoRepeatMovementTimer()
{
	GetWorldTimerManager().SetTimer(AutoRepeatMovementTimerHandle, this, &ATetrisPlayManager::MoveTetrimino, AutoRepeatMovementInterval, bIsAutoRepeatMovementLoop, AutoRepeatMovementInitialDelay);
}

void ATetrisPlayManager::SetSoftDropTimer()
{
	GetWorldTimerManager().SetTimer(SoftDropTimerHandle, this, &ATetrisPlayManager::MoveTetriminoDown, GameMode->GetSoftDropSpeed(), bSoftDropTimerLoop, SoftDropTimerInitialDelay);
}

void ATetrisPlayManager::SetHardDropTimer()
{
	GetWorldTimerManager().SetTimer(HardDropTimerHandle, this, &ATetrisPlayManager::HardDrop, HardDropTimerInitialDelay, bIsHardDropTimerLoop);
}

void ATetrisPlayManager::SetNormalFallTimer()
{
	const bool bIsNormalFallOn = GameMode && !GameMode->bNormalFallOff;
	if (bIsNormalFallOn)
	{
		UE_LOG(LogTemp, Display, TEXT("Normal Fall Timer is set."));
		GetWorldTimerManager().SetTimer(NormalFallTimerHandle, this, &ATetrisPlayManager::MoveTetriminoDown, NormalFallSpeed, bIsNormalFallTimerLoop, NormalFallTimerInitialDelay);
	}
}

void ATetrisPlayManager::SetLockDownTimer(const float FirstDelay)
{
	GetWorldTimerManager().SetTimer(LockDownTimerHandle, this, &ATetrisPlayManager::LockDown, FirstDelay, bIsLockDownTimerLoop);
	UE_LOG(LogTemp, Display, TEXT("LockDown Timer is set."));
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
	// 하드 드롭은 초기 딜레이가 있긴 하지만, 다른 타이머들에 비하면 그 즉시라고 봐야 함.
	// 즉 다른 연산이 끼어들 가능성이 없기 때문에, ClearUserInputTimers()에서는 따로 처리하지 않음.
	// TODO: 사용자가 일시 정지를 누르면 어떡하냐고 생각할 수도 있는데,
	// 그 경우엔 하드 드롭 딜레이가 끝난 후에 일시 정지를 걸거나, 하드 드롭 딜레이 보다 더 많은 딜레이를 주고 일시 정지를 걸면 됨.
	// 아무튼 버그는 아니기 때문에 일단 이렇게 처리한다.
	static const TArray<FTimerHandle*> UserInputTimerHandles =
	{
		&AutoRepeatMovementTimerHandle,
		&SoftDropTimerHandle,
		&NormalFallTimerHandle
	};
	ClearTimers(UserInputTimerHandles);
}

void ATetrisPlayManager::SetTetriminoInPlay(ATetrimino* const NewTetriminoInPlay)
{
	check(NewTetriminoInPlay != nullptr);
	NewTetriminoInPlay->AttachToBoard(Board);
	TetriminoInPlay = NewTetriminoInPlay;
}

ATetrimino* ATetrisPlayManager::PopTetriminoFromNextQueue()
{
	ATetrimino* const NextTetrimino = NextQueue->Dequeue();
	check(NextTetrimino != nullptr);
	SpawnAndPushTetriminoToNextQueue();
	NextQueue->ReArrangeTetriminoLocations();
	return NextTetrimino;
}

void ATetrisPlayManager::SpawnAndPushTetriminoToNextQueue()
{
	ATetrimino* const NewTetrimino = SpawnNextTetrimino();
	check(NewTetrimino != nullptr);
	NextQueue->Enqueue(NewTetrimino);
}

ATetrimino* ATetrisPlayManager::SpawnNextTetrimino() const
{
	static constexpr bool bIsTetriminoSpawnRandom = true;
	if constexpr (bIsTetriminoSpawnRandom)
	{
		return TetriminoGenerator->SpawnTetriminoByBagSystem(TetriminoClass);
	}
	else
	{
		return TetriminoGenerator->SpawnTetriminoByShape(TetriminoClass, TestSpawnShape);
	}
}

void ATetrisPlayManager::PlayLockDownEffect(const TArray<UMino*>& MinoArray)
{
	// TODO: LockDown Effect 추가
	// 파라미터 수정될 여지 있음
}
