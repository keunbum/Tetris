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
	SetTetriminoMovementDirection(FVector2D::ZeroVector);
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
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ATetrisPlayManager::EnterPhase, NewPhase);
	GetWorldTimerManager().SetTimer(PhaseChangeTimerHandle, TimerDelegate, Delay, bIsPhaseChangeTimerLoop);
}

void ATetrisPlayManager::StartMovement(const FVector2D& InMovementDirection)
{
	if (!bIsTetriminoInPlayManipulable)
	{
		UE_LOG(LogTemp, Display, TEXT("Tetrimino is not manipulable."));
		return;
	}

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
	if (!bIsTetriminoInPlayManipulable)
	{
		UE_LOG(LogTemp, Display, TEXT("Tetrimino is not manipulable."));
		return;
	}

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
	if (!bIsTetriminoInPlayManipulable)
	{
		UE_LOG(LogTemp, Display, TEXT("Tetrimino is not manipulable."));
		return;
	}
	HardDrop();
}

void ATetrisPlayManager::DoRotation(const ETetriminoRotationDirection RotationDirection)
{
	if (!bIsTetriminoInPlayManipulable)
	{
		UE_LOG(LogTemp, Display, TEXT("Tetrimino is not manipulable."));
		return;
	}

	RunSuperRotationSystem(RotationDirection);
}

void ATetrisPlayManager::HoldTetriminoInPlay()
{
	if (!bIsTetriminoInPlayManipulable)
	{
		UE_LOG(LogTemp, Display, TEXT("Tetrimino is not manipulable."));
		return;
	}

	if (!IsHoldingTetriminoInPlayAvailable())
	{
		UE_LOG(LogTemp, Display, TEXT("Holding Tetrimino is not available."));
		return;
	}

	bIsTetriminoInPlayManipulable = false;

	// 기존 TetriminoInPlay를 떼어 내기
	if (TetriminoInPlay)
	{
		ATetrimino* const OldTetriminoInPlay = TetriminoInPlay;
		SetTetriminoInPlay(nullptr);

		// HoldQueue에서 테트리미노 가져오기 (비어 있으면 nullptr)
		if (HoldQueue)
		{
			ATetrimino* const TetriminoInHoldQueue = HoldQueue->Dequeue();
			const bool bWasHoldQueueEmpty = (TetriminoInHoldQueue == nullptr);

			// HoldQueue에 기존 TetriminoInPlay 넣기
			HoldQueue->Enqueue(OldTetriminoInPlay);
			OldTetriminoInPlay->RotateByFacing(ATetrimino::DefaultFacing);
			HoldQueue->ReArrangeTetriminoLocations();

			if (bWasHoldQueueEmpty)
			{
				// 비어 있었다면 새로 꺼내고
				RunGenerationPhase();
			}
			else
			{
				// 비어 있지 않았다면 HoldQueue에 있던 테트리미노를 TetriminoInPlay로 설정한다
				SetTetriminoInPlay(TetriminoInHoldQueue);
				RunFallingPhase();
			}
		}
	}

	bIsTetriminoInPlayLockedDownFromLastHold = false;
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
	//UE_LOG(LogTemp, Display, TEXT("Start Generation Phase."));

	ATetrimino* const NewTetriminoInPlay = PopTetriminoFromNextQueue();
	SetTetriminoInPlay(NewTetriminoInPlay);

	if (Board && GameMode)
	{
		// Check Game Over Condition
		// Block Out Condition occurs when part of a newly-generated Tetrimino is blocked due to an existing Block in the Matrix.
		const bool bIsBlockOutCondition = Board->IsBlocked(TetriminoInPlay);
		if (bIsBlockOutCondition)
		{
			UE_LOG(LogTemp, Warning, TEXT("Block Out Condition -> Game Over"));
			GameMode->RunGameOver();
			return;
		}
	}

	// TetriminoInPlay drops one row if no existing Block is in its path.
	bIsTetriminoInPlayManipulable = true;
	MoveTetriminoDown();

	EnterPhase(EPhase::Falling);
}

void ATetrisPlayManager::RunFallingPhase()
{
	//UE_LOG(LogTemp, Display, TEXT("Start Falling Phase."));

	SetNormalFallTimer();
}

void ATetrisPlayManager::RunLockPhase()
{
	//UE_LOG(LogTemp, Display, TEXT("Start Lock Phase."));

	const int32 CurrentLowestRow = TetriminoInPlay->GetLowestRow();
	const FLockDownSystemInfo LockDownSystemInfo(CurrentLowestRow);
	RunLockDownSystem(LockDownSystemInfo);

	SetLockDownTimer();
}

void ATetrisPlayManager::RunPatternPhase()
{
	//UE_LOG(LogTemp, Display, TEXT("Start Pattern Phase."));

	/** Main Logic */
	CheckLineClearPattern(GamePlayInfo.HitList);

	EnterPhase(EPhase::Iterate);
}

void ATetrisPlayManager::RunIteratePhase()
{
	//UE_LOG(LogTemp, Display, TEXT("Start Iterate Phase."));

	/** Main Logic */

	EnterPhase(EPhase::Animate);
}

void ATetrisPlayManager::RunAnimatePhase()
{
	//UE_LOG(LogTemp, Display, TEXT("Start Animate Phase."));

	/** Main Logic */

	EnterPhase(EPhase::Elimate);
}

void ATetrisPlayManager::RunEliminatePhase()
{
	//UE_LOG(LogTemp, Display, TEXT("Start Eliminate Phase."));

	/** Main Logic */
	if (Board)
	{
		Board->ClearRows(GamePlayInfo.HitList);
	}

	EnterPhase(EPhase::Completion);
}

void ATetrisPlayManager::RunCompletionPhase()
{
	//UE_LOG(LogTemp, Display, TEXT("Start Completion Phase."));

	/** Main Logic */
	if (GameMode)
	{
		GameMode->UpdateGamePlay(GamePlayInfo);
	}

	/** Reset Variables */
	GamePlayInfo.Reset();

	EnterPhaseWithDelay(EPhase::Generation, GenerationPhaseChangeInitialDelay);
}

void ATetrisPlayManager::MoveTetriminoTo(const FVector2D& Direction)
{
	if (!TetriminoInPlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayManager::MoveTetriminoTo() - TetriminoInPlay is nullptr."));
		return;
	}

	const FIntPoint MovementIntPoint = ATetriminoBase::GetMatrixMovementIntPointByDirection(Direction);
	const bool bIsMovementPossible = Board->IsMovementPossible(TetriminoInPlay, MovementIntPoint);
	if (bIsMovementPossible)
	{
		TetriminoInPlay->MoveBy(MovementIntPoint);
		EnterLockPhaseIfNecessary();
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

void ATetrisPlayManager::LockDown()
{
	if (!Board || !GameMode || !TetriminoInPlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("Some of the necessary components are nullptr."));
		return;
	}

	bIsTetriminoInPlayManipulable = false;

	PlayLockDownEffect(TetriminoInPlay->GetMinoArray());

	// Game Over Condition
	// Lock Out Condition occurs when a Tetrimino Locks Down completely above the Skyline.
	const bool bIsLockOutCondition = Board->IsAboveSkyline(TetriminoInPlay);
	if (bIsLockOutCondition)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lock Out Condition -> Game Over"));
		GameMode->RunGameOver();
		return;
	}

	// Transfer of TetriminoInPlay's Minos to Board
	TetriminoInPlay->DetachMinos();
	Board->AddMinos(TetriminoInPlay);

	// Remove TetriminoInPlay
	ATetrimino* const OldTetriminoInPlay = TetriminoInPlay;
	SetTetriminoInPlay(nullptr);
	if (OldTetriminoInPlay)
	{
		OldTetriminoInPlay->Destroy();
	}

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
	if (TetriminoInPlay && Board)
	{
		const TArray<FIntPoint>& SRSRotationPointOffsets = TetriminoInPlay->GetSRSRotationPointOffsets(RotationDirection);
		for (const FIntPoint& SRSRotationPointOffset : SRSRotationPointOffsets)
		{
			const bool bIsRotationPossible = Board->IsRotationPossible(TetriminoInPlay, RotationDirection, SRSRotationPointOffset);
			if (bIsRotationPossible)
			{
				TetriminoInPlay->RotateTo(RotationDirection);
				TetriminoInPlay->MoveBy(SRSRotationPointOffset);
				//UE_LOG(LogTemp, Display, TEXT("%Rotation with Point%d."), PointIndex + 1);
				EnterLockPhaseIfNecessary();
				return;
			}
		}
		//UE_LOG(LogTemp, Display, TEXT("All of Rotation Points failed."));
	}
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

void ATetrisPlayManager::SetLockDownSystem(const FLockDownSystemInfo& Info)
{
	ExtendedPlacement.SetLockDownSystem(Info);
}

void ATetrisPlayManager::RunLockDownSystem(const FLockDownSystemInfo& Info)
{
	ExtendedPlacement.RunLockDownSystem(Info);
}

void ATetrisPlayManager::EnterLockPhaseIfNecessary()
{
	if (IsLockPhaseReached())
	{
		if (!GetWorldTimerManager().IsTimerActive(LockDownTimerHandle))
		{
			EnterPhase(EPhase::Lock);
		}
	}
}

bool ATetrisPlayManager::IsHoldingTetriminoInPlayAvailable() const
{
	// 홀드 큐가 비어 있거나, 마지막 홀드로부터 LockDown이 수행된 적이 있다면 가능하다.
	return (HoldQueue && HoldQueue->IsEmpty()) || bIsTetriminoInPlayLockedDownFromLastHold;
}

bool ATetrisPlayManager::IsLockPhaseReached() const
{
	return ExtendedPlacement.IsForcedLockDownReached() || IsTetriminoInPlayOnSurface();
}

bool ATetrisPlayManager::IsTetriminoInPlayOnSurface() const
{
	return Board && Board->IsDirectlyAboveSurface(TetriminoInPlay);
}

void ATetrisPlayManager::MoveTetriminoInPlayToFinalFallingLocation()
{
	if (GhostPiece && TetriminoInPlay)
	{
		const FIntPoint FinalFallingMatrixLocation = GhostPiece->GetMatrixLocation();
		TetriminoInPlay->SetRelativeLocationByMatrixLocation(FinalFallingMatrixLocation);
	}
}

void ATetrisPlayManager::SetAutoRepeatMovementTimer()
{
	GetWorldTimerManager().SetTimer(AutoRepeatMovementTimerHandle, this, &ATetrisPlayManager::MoveTetrimino, AutoRepeatMovementInterval, bIsAutoRepeatMovementLoop, AutoRepeatMovementInitialDelay);
}

void ATetrisPlayManager::SetSoftDropTimer()
{
	const float SoftDropSpeed = ATetrisInGameGameMode::GetSoftDropSpeed(NormalFallSpeed);
	GetWorldTimerManager().SetTimer(SoftDropTimerHandle, this, &ATetrisPlayManager::MoveTetriminoDown, SoftDropSpeed, bSoftDropTimerLoop, SoftDropTimerInitialDelay);
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

void ATetrisPlayManager::ClearAllTimers()
{
	const TArray<FTimerHandle*> TimerHandles =
	{
		&AutoRepeatMovementTimerHandle,
		&SoftDropTimerHandle,
		&NormalFallTimerHandle,
		&LockDownTimerHandle,
		& PhaseChangeTimerHandle,
	};
	ClearTimers(TimerHandles);
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
		ClearAllTimers();
	}
	TetriminoInPlay = InTetriminoInPlay;
}

ATetrimino* ATetrisPlayManager::PopTetriminoFromNextQueue()
{
	if (ATetrimino* const NextTetrimino = NextQueue->Dequeue())
	{
		SpawnAndPushTetriminoToNextQueue();
		NextQueue->ReArrangeTetriminoLocations();
		return NextTetrimino;
	}

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
