// Copyright © 2024 Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayManager.h"

#include "Math/UnrealMathUtility.h"

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
	, bIsTetriminoInPlayLockedDownFromLastHold(false)
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

	// HoldQueue
	HoldQueue = World->SpawnActor<ATetriminoQueue>(ATetriminoQueue::StaticClass());
	check(HoldQueue != nullptr);
	InitializeHoldQueue();

	// GhostPiece
	GhostPiece = World->SpawnActor<AGhostPiece>(GhostPieceClass);
	check(GhostPiece != nullptr);
	GhostPiece->AttachToMatrix(Board->GetMatrixRoot());
}

void ATetrisPlayManager::StartGenerationPhase()
{
	UE_LOG(LogTemp, Display, TEXT("Start Generation Phase."));

	SetPhase(EPhase::Generation);
	ATetrimino* const NewTetriminoInPlay = PopTetriminoFromNextQueue();

	SetTetriminoInPlay(NewTetriminoInPlay);

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

void ATetrisPlayManager::HoldTetriminoInPlay()
{
	// TODO: TetriminoInPlay를 Hold하는 기능 구현하기.
	if (!IsTetriminoInPlayManipulable())
	{
		UE_LOG(LogTemp, Display, TEXT("Tetrimino is not manipulable."));
		return;
	}

	if (!IsHoldingTetriminoInPlayAvailable())
	{
		UE_LOG(LogTemp, Display, TEXT("Holding Tetrimino is not available."));
		return;
	}

	SetIsTetriminoInPlayManipulable(false);

	// 기존 TetriminoInPlay를 때어내기
	ATetrimino* const OldTetriminoInPlay = TetriminoInPlay;
	SetTetriminoInPlay(nullptr);

	// HoldQueue에서 테트리미노 가져오기 (비어 있으면 nullptr)
	ATetrimino* const TetriminoInHoldQueue = HoldQueue->Dequeue();
	const bool bWasHoldQueueEmpty = (TetriminoInHoldQueue == nullptr);

	// HoldQueue에 기존 TetriminoInPlay 넣기
	HoldQueue->Enqueue(OldTetriminoInPlay);
	OldTetriminoInPlay->RotateByFacing(ATetrimino::DefaultFacing);

	HoldQueue->ReArrangeTetriminoLocations();

	if (bWasHoldQueueEmpty)
	{
		// 비어 있었다면 새로 꺼내고
		StartGenerationPhase();
	}
	else
	{
		// 비어 있지 않았다면 HoldQueue에 있던 테트리미노를 TetriminoInPlay로 설정한다
		SetTetriminoInPlay(TetriminoInHoldQueue);
		StartFallingPhase();
	}

	bIsTetriminoInPlayLockedDownFromLastHold = false;
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

void ATetrisPlayManager::InitializeHoldQueue()
{
	// TODO: HoldQueue 초기화 구현하기.
	HoldQueue->Initialize(GameMode->HoldQueueSize, Board->GetHoldQueueRoot());
	check(HoldQueue->IsEmpty());
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

void ATetrisPlayManager::StartLockPhase(const float LockDownFirstDelay)
{
	UE_LOG(LogTemp, Display, TEXT("Start Lock Phase."));

	SetPhase(EPhase::Lock);

	// LockDownFirstDelay가 0.0f이면 LockDown 메서드를 바로 호출하고 (직접 비교 말고 오차 범위 고려해서 비교하기),
	// 그렇지 않으면 LockDown 타이머를 설정한다. (0.0f를 타이머 함수에 넘겨주면 이상한 일이 일어나서 이렇게 구현함.)
	if (FMath::IsNearlyZero(LockDownFirstDelay))
	{
		LockDown();
	}
	else
	{
		SetLockDownTimer(LockDownFirstDelay);
	}
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
			StartLockPhase(LockDownTimerInitialDelayOfNormalFallOrSoftDrop);
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

bool ATetrisPlayManager::IsHoldingTetriminoInPlayAvailable() const
{
	// 홀드 큐가 비어 있거나, 마지막 홀드로부터 LockDown이 수행된 적이 있다면 가능하다.
	return HoldQueue->IsEmpty() || bIsTetriminoInPlayLockedDownFromLastHold;
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

	// Transfer of TetriminoInPlay's Minos to Board
	TetriminoInPlay->DetachMinos();
	Board->AddMinos(TetriminoInPlay);

	// Remove TetriminoInPlay
	ATetrimino* const OldTetriminoInPlay = TetriminoInPlay;
	SetTetriminoInPlay(nullptr);
	OldTetriminoInPlay->Destroy();

	bIsTetriminoInPlayLockedDownFromLastHold = true;

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

	StartLockPhase(LockDownDelayOfHardDrop);
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

void ATetrisPlayManager::ClearTetriminoInPlayLogicTimers()
{
	static const TArray<FTimerHandle*> TimerHandlesToClear =
	{
		&AutoRepeatMovementTimerHandle,
		&NormalFallTimerHandle,
		&LockDownTimerHandle,
		&SoftDropTimerHandle,
		&HardDropTimerHandle,
	};
	ClearTimers(TimerHandlesToClear);
}

void ATetrisPlayManager::SetTetriminoInPlay(ATetrimino* const InTetriminoInPlay)
{
	if (InTetriminoInPlay)
	{
		InTetriminoInPlay->SetBoard(Board);
		InTetriminoInPlay->SetGhostPiece(GhostPiece);
	}
	else
	{
		TetriminoInPlay->SetGhostPiece(nullptr);
		TetriminoInPlay->SetBoard(nullptr);
		ClearTetriminoInPlayLogicTimers();
	}
	TetriminoInPlay = InTetriminoInPlay;
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
