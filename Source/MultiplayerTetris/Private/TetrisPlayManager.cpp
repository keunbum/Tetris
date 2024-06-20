// Copyright KeunBeom Ryu. All Rights Reserved.

#include "TetrisPlayManager.h"

#include "Board.h"
#include "TetrisGameModeBase.h"
#include "Tetrimino.h"

ATetrisPlayManager::ATetrisPlayManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NormalFallSpeed = -1.0f;

	TetriminoClass = ATetrimino::StaticClass();

	TetriminoInPlay = nullptr;
}

void ATetrisPlayManager::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATetrisPlayManager::StartGenerationPhase()
{
	ATetrimino* const NewTetrimino = SpawnNextTetrimino();
	check(NewTetrimino != nullptr);
	ChangeTetrimino(NewTetrimino);
}

void ATetrisPlayManager::StartMovement(const FVector2D& InMovementDirection)
{
	SetMovementDirection(InMovementDirection);
	MoveTetriminoToCurrentDirection();
	SetAutoRepeatMovement();
}

void ATetrisPlayManager::EndMovement()
{
	ClearTimer(AutoRepeatMovementTimerHandle);
	SetMovementDirection(FVector2D::ZeroVector);
}

void ATetrisPlayManager::StartSoftDrop()
{
	if (TetriminoInPlay)
	{
		// NormalFall 일시 중지
		ClearTimer(NormalFallTimerHandle);

		GetWorldTimerManager().SetTimer(SoftDropTimerHandle, this, &ATetrisPlayManager::MoveTetriminoDown, GameMode->GetSoftDropSpeed(), bSoftDropTimerLoop, SoftDropTimerInitialDelay);
	}
}

void ATetrisPlayManager::EndSoftDrop()
{
	ClearTimer(SoftDropTimerHandle);

	// NormalFall 재개
	SetNormalFallTimer();
}

void ATetrisPlayManager::DoHardDrop()
{
	// TODO: 하드 드롭 로직 추가
}

void ATetrisPlayManager::DoRotation(const ETetriminoRotationDirection RotationDirection)
{
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

	SetNormalFallTimer();
	SetMovementDirection(FVector2D::ZeroVector);

	Board = World->SpawnActor<ABoard>();
	check(Board != nullptr);

	UMino::ClearMaterialCache();
}

void ATetrisPlayManager::ClearTimer(FTimerHandle& InOutTimerHandle)
{
	GetWorldTimerManager().ClearTimer(InOutTimerHandle);
}

void ATetrisPlayManager::MoveTetriminoTo(const FVector2D& Direction)
{
	if (TetriminoInPlay)
	{
		static constexpr float OneSpace = 1.0f;
		const FVector2D MovementVector2D = OneSpace * Direction;
		const FIntPoint MovementIntVector2D(static_cast<int32>(MovementVector2D.X), static_cast<int32>(MovementVector2D.Y));
		if (Board->IsMovementPossible(TetriminoInPlay, MovementIntVector2D))
		{
			TetriminoInPlay->MoveBy(MovementIntVector2D);
		}
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

void ATetrisPlayManager::SetAutoRepeatMovement()
{
	GetWorldTimerManager().SetTimer(AutoRepeatMovementTimerHandle, this, &ATetrisPlayManager::MoveTetriminoToCurrentDirection, AutoRepeatMovementInterval, bIsAutoRepeatMovementLoop, AutoRepeatMovementInitialDelay);
}

void ATetrisPlayManager::SetNormalFallTimer()
{
	if (GameMode && !GameMode->bNormalFallOff)
	{
		GetWorldTimerManager().SetTimer(NormalFallTimerHandle, this, &ATetrisPlayManager::MoveTetriminoDown, NormalFallSpeed, bIsNormalFallTimerLoop, NormalFallTimerInitialDelay);
	}
}

ATetrimino* ATetrisPlayManager::SpawnNextTetrimino() const
{
	if (ATetrimino* const NewTetrimino = GetWorld()->SpawnActor<ATetrimino>(TetriminoClass))
	{
//#define TETRIMINO_SPAWN_RANDOM

#ifdef TETRIMINO_SPAWN_RANDOM
		const ETetriminoShape NewTetriminoType = ATetrimino::GetTetriminoShapeRandom();
		NewTetrimino->Initialize(NewTetriminoType);
#else
		NewTetrimino->Initialize(TestSpawnType);
#endif
		return NewTetrimino;
	}
	return nullptr;
}

void ATetrisPlayManager::ChangeTetrimino(ATetrimino* const NewTetrimino)
{
	check(NewTetrimino != nullptr);
	NewTetrimino->AttachToBoard(Board);
	SetTetriminoInPlay(NewTetrimino);
}
