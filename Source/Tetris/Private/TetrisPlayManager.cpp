// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayManager.h"

#include "Math/UnrealMathUtility.h"

#include "Board.h"
#include "TetrisInGameGameMode.h"
#include "Tetrimino.h"
#include "GhostPiece.h"
#include "TetrisPlayerControllerSingle.h"
#include "TetriminoGenerator.h"
#include "TetriminoQueue.h"

ATetrisPlayManager::ATetrisPlayManager()
	: Phase(EPhase::None)
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
	UE_LOG(LogTemp, Display, TEXT("TetrisPlayManager is initialized()"));

	// World
	UWorld* const World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is nullptr."));
		return;
	}

	// GameMode
	GameMode = World->GetAuthGameMode<ATetrisInGameGameMode>();
	if (GameMode)
	{
		SetNormalFallSpeed(GameMode->GetCurrentLevelNormalFallSpeed());
	}

	// Board
	Board = World->SpawnActor<ABoard>(BoardClass);
	if (Board)
	{
		Board->Initialize();

		// GhostPiece
		GhostPiece = World->SpawnActor<AGhostPiece>(GhostPieceClass);
		if (GhostPiece)
		{
			GhostPiece->AttachToMatrix(Board->GetMatrixRoot());
		}
	}

	// TetriminoGenerator
	TetriminoGenerator = NewObject<UTetriminoGenerator>(this);

	// NextQueue
	NextQueue = World->SpawnActor<ATetriminoQueue>(ATetriminoQueue::StaticClass());
	InitializeNextQueue();

	// HoldQueue
	HoldQueue = World->SpawnActor<ATetriminoQueue>(ATetriminoQueue::StaticClass());
	InitializeHoldQueue();

	// Etc
	SetAutoRepeatMovementDirection(FVector2D::ZeroVector);

	ClearAllTimers();
}

void ATetrisPlayManager::EnterPhase(const EPhase NewPhase)
{
	Phase = NewPhase;

	switch (Phase)
	{
	case EPhase::Generation:
		RunGenerationPhase();
		break;
	case EPhase::Falling:
		RunFallingPhase();
		break;
	case EPhase::Lock:
		RunLockPhase();
		break;
	case EPhase::Pattern:
		RunPatternPhase();
		break;
	case EPhase::Iterate:
		RunIteratePhase();
		break;
	case EPhase::Animate:
		RunAnimatePhase();
		break;
	case EPhase::Elimate:
		RunEliminatePhase();
		break;
	case EPhase::Completion:
		RunCompletionPhase();
		break;
	default:
		break;
	}
}

void ATetrisPlayManager::EnterPhaseWithDelay(const EPhase NewPhase, const float Delay)
{
	FTimerHandle PhaseChangeTimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ATetrisPlayManager::EnterPhase, NewPhase);
	GetWorldTimerManager().SetTimer(PhaseChangeTimerHandle, TimerDelegate, Delay, bIsPhaseChangeTimerLoop);
}

void ATetrisPlayManager::StartAutoRepeatMovement(const FVector2D& InMovementDirection)
{
	SetAutoRepeatMovementDirection(InMovementDirection);
	MoveTetrimino();
	SetAutoRepeatMovementTimer();
}

void ATetrisPlayManager::EndAutoRepeatMovement()
{
	ClearTimer(AutoRepeatMovementTimerHandle);
	SetAutoRepeatMovementDirection(FVector2D::ZeroVector);
}

void ATetrisPlayManager::StartSoftDrop()
{
	// NormalFall 일시 중지
	ClearTimer(NormalFallTimerHandle);
	SetSoftDropTimer();
}

void ATetrisPlayManager::EndSoftDrop()
{
	ClearTimer(SoftDropTimerHandle);
	// NormalFall 재개
	SetNormalFallTimer();
}

void ATetrisPlayManager::DoHardDrop()
{
	// 테트리스 가이드라인 2009에 나와 있는 방식 그대로 구현하기.
	// HardDrop에는 Auto-Repeat 없음.
	// TODO: 나중에 Hard Drop Trail 관련 이펙트도 있으면 금상첨화.
	if (!bIsTetriminoInPlayManipulable)
	{
		UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::DoHardDrop() - Tetrimino is not manipulable."));
		return;
	}
	HardDrop();
}

void ATetrisPlayManager::DoRotation(const ETetriminoRotationDirection RotationDirection)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::DoRotation()"));

	if (!bIsTetriminoInPlayManipulable)
	{
		UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::DoRotation() - Tetrimino is not manipulable."));
		return;
	}

	RunSuperRotationSystem(RotationDirection);
}

void ATetrisPlayManager::HoldTetriminoInPlay()
{
	if (!bIsTetriminoInPlayManipulable)
	{
		UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::HoldTetriminoInPlay() - Tetrimino is not manipulable."));
		return;
	}

	if (!IsHoldingTetriminoInPlayAvailable())
	{
		UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::HoldTetriminoInPlay() - Holding Tetrimino is not available."));
		return;
	}

	if (!ensureMsgf(TetriminoInPlay && HoldQueue, TEXT("Some of the necessary components are nullptr.")))
	{
		return;
	}

	bIsTetriminoInPlayManipulable = false;

	// HoldQueue에서 테트리미노 가져오기 (비어 있으면 nullptr)
	ATetrimino* const TetriminoFromHoldQueue = HoldQueue->Dequeue();

	// HoldQueue에 기존 TetriminoInPlay 넣기
	TetriminoInPlay->DetachFromBoard();
	TetriminoInPlay->RotateByFacing(ATetrimino::DefaultFacing);
	HoldQueue->Enqueue(TetriminoInPlay);
	HoldQueue->ReArrangeTetriminoLocations();

	SetTetriminoInPlay(TetriminoFromHoldQueue);
	bIsTetriminoInPlayLockedDownFromLastHold = false;

	EnterPhase(EPhase::Generation);
}

FName ATetrisPlayManager::GetPhaseName(const EPhase Phase)
{
	static const TMap<EPhase, FName> PhaseNameMap =
	{
		{EPhase::None, "None"},
		{EPhase::Generation, "Generation"},
		{EPhase::Falling, "Falling"},
		{EPhase::Lock, "Lock"},
		{EPhase::Pattern, "Pattern"},
		{EPhase::Iterate, "Iterate"},
		{EPhase::Animate, "Animate"},
		{EPhase::Elimate, "Elimate"},
		{EPhase::Completion, "Completion"},
	};

	return PhaseNameMap[Phase];
}

void ATetrisPlayManager::InitializeNextQueue()
{
	if (NextQueue && GameMode && Board)
	{
		NextQueue->Initialize(GameMode->NextQueueSize, Board->GetNextQueueRoot());
		for (int32 Count = 0; Count < GameMode->NextQueueSize; ++Count)
		{
			SpawnAndPushTetriminoToNextQueue();
		}
		NextQueue->ReArrangeTetriminoLocations();
	}
}

void ATetrisPlayManager::InitializeHoldQueue()
{
	if (HoldQueue && GameMode && Board)
	{
		HoldQueue->Initialize(GameMode->HoldQueueSize, Board->GetHoldQueueRoot());
	}
}

void ATetrisPlayManager::RunGenerationPhase()
{
	//UE_LOG(LogTemp, Display, TEXT("|-----------------------------------------------------------------------|"));
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunGenerationPhase()"));

	// TetriminoInPlay가 비어 있으면 NextQueue에서 꺼내온다. (HoldQueue에서 꺼내 왔다면 가져오지 않는다.)
	if (!TetriminoInPlay)
	{
		ATetrimino* const NewTetriminoInPlay = PopTetriminoFromNextQueue();
		SetTetriminoInPlay(NewTetriminoInPlay);
	}

	if (ensureMsgf(Board && GameMode, TEXT("Some of the necessary components are nullptr.")))
	{
		// Check Game Over Condition
		// Block Out Condition occurs when part of a newly-generated Tetrimino is blocked due to an existing Block in the Matrix.
		const bool bIsBlockOutCondition = Board->IsBlocked(TetriminoInPlay);
		if (bIsBlockOutCondition)
		{
			//UE_LOG(LogTemp, Display, TEXT("Block Out Condition -> Game Over"));
			GameMode->RunGameOver();
			return;
		}
	}

	// TetriminoInPlay drops one row if no existing Block is in its path.
	MoveTetriminoToInternal(ATetriminoBase::MoveDirectionDown);

	EnterPhase(EPhase::Falling);
}

void ATetrisPlayManager::RunFallingPhase()
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunFallingPhase()"));

	bIsTetriminoInPlayManipulable = true;

	SetNormalFallTimer();
}

void ATetrisPlayManager::RunLockPhase()
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunLockPhase()"));

	SetLockDownTimer();
}

void ATetrisPlayManager::RunPatternPhase()
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunPatternPhase()"));

	/** Main Logic */
	CheckLineClearPattern(GamePlayInfo.HitList);

	EnterPhase(EPhase::Iterate);
}

void ATetrisPlayManager::RunIteratePhase()
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunIteratePhase()"));

	/** Main Logic */

	EnterPhase(EPhase::Animate);
}

void ATetrisPlayManager::RunAnimatePhase()
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunAnimatePhase()"));

	/** Main Logic */

	EnterPhase(EPhase::Elimate);
}

void ATetrisPlayManager::RunEliminatePhase()
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunEliminatePhase()"));

	/** Main Logic */
	if (Board)
	{
		Board->ClearRows(GamePlayInfo.HitList);
	}

	EnterPhase(EPhase::Completion);
}

void ATetrisPlayManager::RunCompletionPhase()
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunCompletionPhase()"));

	/** Main Logic */
	if (GameMode)
	{
		GameMode->UpdateGamePlay(GamePlayInfo);
	}

	/** Reset */
	GamePlayInfo.Reset();
	check(!IsTimerActive(LockDownTimerHandle));
	ClearTimers({ &NormalFallTimerHandle });

	EnterPhaseWithDelay(EPhase::Generation, GenerationPhaseChangeInitialDelay);
}

void ATetrisPlayManager::MoveTetriminoTo(const FVector2D& Direction)
{
	if (!bIsTetriminoInPlayManipulable)
	{
		UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::MoveTetriminoTo() - Tetrimino is not manipulable."));
		return;
	}

	if (!ensureMsgf(TetriminoInPlay, TEXT("ATetrisPlayManager::MoveTetriminoTo() - TetriminoInPlay is nullptr.")))
	{
		return;
	}

	MoveTetriminoToInternal(Direction);
}

void ATetrisPlayManager::MoveTetrimino()
{
	MoveTetriminoTo(AutoRepeatMovementDirection);
}

void ATetrisPlayManager::MoveTetriminoDown()
{
	MoveTetriminoTo(ATetriminoBase::MoveDirectionDown);
}

void ATetrisPlayManager::MoveTetriminoInPlayToFinalFallingLocation()
{
	if (GhostPiece && TetriminoInPlay)
	{
		const FIntPoint FinalFallingMatrixLocation = GhostPiece->GetMatrixLocation();
		TetriminoInPlay->SetRelativeLocationByMatrixLocation(FinalFallingMatrixLocation);
	}
}

void ATetrisPlayManager::MoveTetriminoToInternal(const FVector2D& Direction)
{
	if (!ensureMsgf(Board, TEXT("ATetrisPlayManager::MoveTetriminoTo() - Board is nullptr.")))
	{
		return;
	}

	const FIntPoint MovementIntPoint = ATetriminoBase::GetMatrixMovementIntPointByDirection(Direction);
	const bool bIsMovementPossible = Board->IsMovementPossible(TetriminoInPlay, MovementIntPoint);
	if (bIsMovementPossible)
	{
		TetriminoInPlay->MoveBy(MovementIntPoint);
		RunLockDownSystem();
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::MoveTetriminoTo() - Movement is not possible."));
	}
}

void ATetrisPlayManager::LockDown()
{
	if (IsTimerActive(LockDownTimerHandle))
	{
		//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::LockDown() - LockDownTimer is active, so it will be cleared."));
		ClearTimer(LockDownTimerHandle);
	}

	if (!IsTetriminoInPlayOnSurface())
	{
		//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::LockDown() - TetriminoInPlay is not on the surface. So Do nothing."));
		return;
	}

	bIsTetriminoInPlayManipulable = false;

	if (!ensureMsgf(Board && TetriminoInPlay && GameMode, TEXT("Some of the necessary components are nullptr.")))
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayManager::LockDown()"));

	PlayLockDownEffect(TetriminoInPlay->GetMinoArray());

	// Game Over Condition
	// Lock Out Condition occurs when a Tetrimino Locks Down completely above the Skyline.
	const bool bIsLockOutCondition = Board->IsAboveSkyline(TetriminoInPlay);
	if (bIsLockOutCondition)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Lock Out Condition -> Game Over"));
		GameMode->RunGameOver();
		return;
	}

	// Transfer of TetriminoInPlay's Minos to Board
	TetriminoInPlay->DetachMinos();
	Board->AddMinos(TetriminoInPlay);
	RemoveTetriminoInPlay();

	bIsTetriminoInPlayLockedDownFromLastHold = true;

	EnterPhase(EPhase::Pattern);
}

void ATetrisPlayManager::ForceLockDown()
{
	ClearTimer(LockDownTimerHandle);
	LockDown();
}

void ATetrisPlayManager::HardDrop()
{
	// GhostPiece를 잠시 안보이게 한다.
	if (GhostPiece)
	{
		GhostPiece->SetActorHiddenInGame(true);
	}
	MoveTetriminoInPlayToFinalFallingLocation();
	ForceLockDown();
}

void ATetrisPlayManager::RunSuperRotationSystem(const ETetriminoRotationDirection RotationDirection)
{
	if (!TetriminoInPlay || !Board)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayManager::RunSuperRotationSystem() - TetriminoInPlay or Board is nullptr."));
		return;
	}

	const TArray<FIntPoint>& SRSRotationPointOffsets = TetriminoInPlay->GetSRSRotationPointOffsets(RotationDirection);
	for (const FIntPoint& SRSRotationPointOffset : SRSRotationPointOffsets)
	{
		const bool bIsRotationPossible = Board->IsRotationPossible(TetriminoInPlay, RotationDirection, SRSRotationPointOffset);
		if (bIsRotationPossible)
		{
			TetriminoInPlay->RotateToWithPointOffset(RotationDirection, SRSRotationPointOffset);
			//UE_LOG(LogTemp, Display, TEXT("%Rotation with Point%d."), PointIndex + 1);
			RunLockDownSystem();
			return;
		}
	}
	//UE_LOG(LogTemp, Display, TEXT("All of Rotation Points failed."));
}

void ATetrisPlayManager::CheckLineClearPattern(TArray<int32>& OutHitList)
{
	// 모든 대상 행에 대해 LineClearPattern을 체크해서 HitList에 추가한다.
	// RowIndex 범위 아마 Visible이 맞을 거임. 스카이라인 위에서 라인 클리어 패턴이 발견될 일 없음. 그전에 게임 오버로 끝났어야 함.
	// TODO: 근데 가이드라인을 정독한 게 아니므로, 틀릴 수도 있음.
	if (Board)
	{
		for (int32 RowIndex = ABoard::VisibleBeginRow; RowIndex < ABoard::VisibleEndRow; ++RowIndex)
		{
			const bool bIsLineClearPattern = Board->IsRowFull(RowIndex);
			if (bIsLineClearPattern)
			{
				OutHitList.Add(RowIndex);
			}
		}
	}
}

void ATetrisPlayManager::RunLockDownSystem()
{
	const int32 CurrentRow = TetriminoInPlay->GetLowestRow();
	if (CurrentRow > ExtendedPlacement.LowestRow)
	{
		//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunLockDownSystem() -> It's a new lowest row: %d"), CurrentRow);
		ExtendedPlacement.LowestRow = CurrentRow;
		ExtendedPlacement.TimerResetCount = ExtendedPlacement.MaxTimerResetCount;
	}

	if (IsTetriminoInPlayOnSurface())
	{
		//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunLockDownSystem() -> Tetrimino is on the surface."));
		if (ExtendedPlacement.TimerResetCount <= 0)
		{
			//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunLockDownSystem() -> Force Lock Down"));
			ForceLockDown();
		}
		else
		{
			if (IsTimerActive(LockDownTimerHandle))
			{
				//ClearTimer(LockDownTimerHandle);
				--ExtendedPlacement.TimerResetCount;
				//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunLockDownSystem() -> TimerResetCount Used: %d"), ExtendedPlacement.TimerResetCount);
			}
			EnterPhase(EPhase::Lock);
		}
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunLockDownSystem() -> Tetrimino is not on the surface."));
		if (IsTimerActive(LockDownTimerHandle))
		{
			//ClearTimer(LockDownTimerHandle);
			--ExtendedPlacement.TimerResetCount;
			//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayManager::RunLockDownSystem() -> TimerResetCount Used: %d"), ExtendedPlacement.TimerResetCount);
		}
	}
}

void ATetrisPlayManager::RemoveTetriminoInPlay()
{
	if (TetriminoInPlay)
	{
		TetriminoInPlay->DetachFromBoard();
		TetriminoInPlay->Destroy();
		TetriminoInPlay = nullptr;
	}
}

bool ATetrisPlayManager::IsHoldingTetriminoInPlayAvailable() const
{
	// 홀드 큐가 비어 있거나, 마지막 홀드로부터 LockDown이 수행된 적이 있다면 가능하다.
	return (HoldQueue && HoldQueue->IsEmpty()) || bIsTetriminoInPlayLockedDownFromLastHold;
}

bool ATetrisPlayManager::IsTetriminoInPlayOnSurface() const
{
	return Board && Board->IsDirectlyAboveSurface(TetriminoInPlay);
}

void ATetrisPlayManager::SetAutoRepeatMovementTimer()
{
	GetWorldTimerManager().SetTimer(AutoRepeatMovementTimerHandle, this, &ATetrisPlayManager::MoveTetrimino, AutoRepeatMovementInterval, bIsAutoRepeatMovementLoop, AutoRepeatMovementInitialDelay);
}

void ATetrisPlayManager::SetSoftDropTimer()
{
	const float SoftDropSpeed = ATetrisInGameGameMode::GetSoftDropSpeed(NormalFallSpeed);
	GetWorldTimerManager().SetTimer(SoftDropTimerHandle, this, &ATetrisPlayManager::MoveTetriminoDown, SoftDropSpeed, bSoftDropTimerLoop, SoftDropTimerInitialDelay);
	//UE_LOG(LogTemp, Display, TEXT("Soft Drop Timer is set."));
}

void ATetrisPlayManager::SetNormalFallTimer()
{
	const bool bIsNormalFallOn = GameMode && !GameMode->bNormalFallOff;
	if (bIsNormalFallOn)
	{
		GetWorldTimerManager().SetTimer(NormalFallTimerHandle, this, &ATetrisPlayManager::MoveTetriminoDown, NormalFallSpeed, bIsNormalFallTimerLoop, NormalFallTimerInitialDelay);
		//UE_LOG(LogTemp, Display, TEXT("Normal Fall Timer is set."));
	}
}

void ATetrisPlayManager::SetLockDownTimer()
{
	GetWorldTimerManager().SetTimer(LockDownTimerHandle, this, &ATetrisPlayManager::LockDown, LockDownTimerInitialDelay, bIsLockDownTimerLoop);
	//UE_LOG(LogTemp, Display, TEXT("Lock Down Timer is set."));
}

void ATetrisPlayManager::ClearTimer(FTimerHandle& InOutTimerHandle)
{
	GetWorldTimerManager().ClearTimer(InOutTimerHandle);
}

void ATetrisPlayManager::ClearTimerWithPrefix(const FString& Prefix, FTimerHandle& InOutTimerHandle)
{
	const FString TimerIsActive = IsTimerActive(InOutTimerHandle) ? TEXT("o") : TEXT("x");
	UE_LOG(LogTemp, Display, TEXT("Before ClearTimer(): %s Timer is %s."), *Prefix, *TimerIsActive);
	ClearTimer(InOutTimerHandle);
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

void ATetrisPlayManager::ClearAllTimers()
{
	const TArray<FTimerHandle*> TimerHandles =
	{
		&AutoRepeatMovementTimerHandle,
		&SoftDropTimerHandle,
		&NormalFallTimerHandle,
		&LockDownTimerHandle,
	};
	ClearTimers(TimerHandles);

	//ClearTimerWithPrefix(TEXT("Auto Repeat Movement"), AutoRepeatMovementTimerHandle);
	//ClearTimerWithPrefix(TEXT("Soft Drop"), SoftDropTimerHandle);
	//ClearTimerWithPrefix(TEXT("Normal Fall"), NormalFallTimerHandle);
	//ClearTimerWithPrefix(TEXT("Lock Down"), LockDownTimerHandle);
	//ClearTimerWithPrefix(TEXT("Phase Change"), PhaseChangeTimerHandle);
}

bool ATetrisPlayManager::IsTimerActive(const FTimerHandle& TimerHandle) const
{
	return GetWorldTimerManager().IsTimerActive(TimerHandle);
}

void ATetrisPlayManager::SetTetriminoInPlay(ATetrimino* const InTetriminoInPlay)
{
	TetriminoInPlay = InTetriminoInPlay;
	if (TetriminoInPlay)
	{
		TetriminoInPlay->SetBoard(Board);
		TetriminoInPlay->SetGhostPiece(GhostPiece);
		ExtendedPlacement.Init(TetriminoInPlay->GetLowestRow());
	}
}

ATetrimino* ATetrisPlayManager::PopTetriminoFromNextQueue()
{
	if (ATetrimino* const NextTetrimino = NextQueue->Dequeue())
	{
		SpawnAndPushTetriminoToNextQueue();
		NextQueue->ReArrangeTetriminoLocations();
		return NextTetrimino;
	}

	checkNoEntry();
	return nullptr;
}

void ATetrisPlayManager::SpawnAndPushTetriminoToNextQueue()
{
	if (ATetrimino* const NewTetrimino = SpawnNextTetrimino())
	{
		NextQueue->Enqueue(NewTetrimino);
	}
}

ATetrimino* ATetrisPlayManager::SpawnNextTetrimino() const
{
	if (TetriminoGenerator)
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

	return nullptr;
}

void ATetrisPlayManager::PlayLockDownEffect(const TArray<UMino*>& MinoArray)
{
	// TODO: LockDown Effect 추가
	// 파라미터 수정될 여지 있음
}
