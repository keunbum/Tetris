// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisPlayManager.h"

#include "Board.h"
#include "TetrisGameModeBase.h"
#include "Tetrimino.h"

// Sets default values
ATetrisPlayManager::ATetrisPlayManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NormalFallSpeed = -1.0f;

	TetriminoClass = ATetrimino::StaticClass();

	TetriminoInPlay = nullptr;
}

// Called when the game starts or when spawned
void ATetrisPlayManager::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

// Called every frame
void ATetrisPlayManager::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATetrisPlayManager::StartGenerationPhase()
{
	ATetrimino* const NewTetrimino = SpawnNextTetrimino();
	check(NewTetrimino != nullptr);
	AttachTetrimino(NewTetrimino);
}

void ATetrisPlayManager::StartMovement(const FVector2D& InMovementDirection)
{
	SetMovementDirection(InMovementDirection);
	MoveToCurrentDirection();
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

		GetWorldTimerManager().SetTimer(SoftDropTimerHandle, this, &ATetrisPlayManager::MoveDown, GameMode->GetSoftDropSpeed(), bSoftDropTimerLoop, SoftDropTimerInitialDelay);
	}
}

void ATetrisPlayManager::EndSoftDrop()
{
	ClearTimer(SoftDropTimerHandle);

	// NormalFall 재개
	SetNormalFallTimer();
}

void ATetrisPlayManager::StartHardDrop()
{
	// TODO: 하드 드롭 로직 추가
}

void ATetrisPlayManager::StartRotate(const int32 RotationDirection)
{
	if (TetriminoInPlay)
	{
		TetriminoInPlay->RotateBy(RotationDirection);
	}
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
}

void ATetrisPlayManager::ClearTimer(FTimerHandle& InOutTimerHandle)
{
	GetWorldTimerManager().ClearTimer(InOutTimerHandle);
}

void ATetrisPlayManager::MoveTo(const FVector2D& Direction)
{
	if (TetriminoInPlay)
	{
		const bool bIsNextPositionPossible = true;
		if (bIsNextPositionPossible)
		{
			TetriminoInPlay->MoveBy(Direction);
		}
	}
}

void ATetrisPlayManager::MoveToCurrentDirection()
{
	MoveTo(MovementDirection);
}

void ATetrisPlayManager::MoveDown()
{
	MoveTo(ATetrimino::MoveDirectionDown);
}

void ATetrisPlayManager::SetAutoRepeatMovement()
{
	GetWorldTimerManager().SetTimer(AutoRepeatMovementTimerHandle, this, &ATetrisPlayManager::MoveToCurrentDirection, AutoRepeatMovementInterval, bIsAutoRepeatMovementLoop, AutoRepeatMovementInitialDelay);
}

void ATetrisPlayManager::SetNormalFallTimer()
{
	if (GameMode && !GameMode->bNormalFallOff)
	{
		GetWorldTimerManager().SetTimer(NormalFallTimerHandle, this, &ATetrisPlayManager::MoveDown, NormalFallSpeed, bIsNormalFallTimerLoop, NormalFallTimerInitialDelay);
	}
}

ATetrimino* ATetrisPlayManager::SpawnNextTetrimino() const
{
	if (ATetrimino* const NewTetrimino = GetWorld()->SpawnActor<ATetrimino>(TetriminoClass))
	{
		const ETetriminoShape NewTetriminoType = ATetrimino::GetTetriminoShapeRandom();
		NewTetrimino->Initialize(NewTetriminoType);
		return NewTetrimino;
	}
	return nullptr;
}

void ATetrisPlayManager::AttachTetrimino(ATetrimino* const NewTetrimino)
{
	check(NewTetrimino != nullptr);
	Board->AttachTetrimino(NewTetrimino);
	SetTetrominoInPlay(NewTetrimino);
}
