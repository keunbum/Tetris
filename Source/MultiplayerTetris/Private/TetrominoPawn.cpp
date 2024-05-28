// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "TetrominoPawn.h"

#include "Board.h"
#include "TetrisGameModeBase.h"
#include "Tetromino.h"

// Sets default values
ATetrominoPawn::ATetrominoPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATetrominoPawn::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

// Called every frame
void ATetrominoPawn::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATetrominoPawn::SetupPlayerInputComponent(UInputComponent* const PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATetrominoPawn::SetTetrominoInPlay(ATetromino* const NewTetrominoInPlay)
{
	TetrominoInPlay = NewTetrominoInPlay;
}

void ATetrominoPawn::UpdateNormalFallSpeed(const float NewNormalFallSpeed)
{
	GetWorldTimerManager().SetTimer(NormalFallTimerHandle, this, &ATetrominoPawn::NormalFall, NewNormalFallSpeed, bIsNormalFallTimerLoop, NormalFallTimerInitialDelay);
}

void ATetrominoPawn::OnMove(const FVector2D& InMovementDirection)
{
	SetMovementDirection(InMovementDirection);
	MoveToCurrentDirection();
	SetAutoRepeatMovement();
}

void ATetrominoPawn::OnEndMove()
{
	ClearTimer(AutoRepeatMovementTimerHandle);
}

void ATetrominoPawn::OnSoftDrop()
{
	if (TetrominoInPlay)
	{
		// NormalFall 일시 중지
		ClearTimer(NormalFallTimerHandle);

		GetWorldTimerManager().SetTimer(SoftDropTimerHandle, this, &ATetrominoPawn::MoveDown, GameMode->GetSoftDropSpeed(), bSoftDropTimerLoop, SoftDropTimerInitialDelay);
	}
}

void ATetrominoPawn::OnEndSoftDrop()
{
	ClearTimer(SoftDropTimerHandle);

	// NormalFall 재개
	SetNormalFallTimer();
}

void ATetrominoPawn::OnHardDrop()
{
	// TODO: 하드 드롭 로직 추가
}

void ATetrominoPawn::Initialize()
{
	GameMode = GetWorld()->GetAuthGameMode<ATetrisGameModeBase>();
	SetNormalFallTimer();
	MovementDirection = FVector2D::ZeroVector;
}

void ATetrominoPawn::ClearTimer(FTimerHandle& InOutTimerHandle)
{
	GetWorldTimerManager().ClearTimer(InOutTimerHandle);
}

void ATetrominoPawn::MoveTo(const FVector2D& Direction)
{
	if (TetrominoInPlay)
	{
		TetrominoInPlay->Move(Direction);
	}
}

void ATetrominoPawn::MoveToCurrentDirection()
{
	MoveTo(MovementDirection);
}

void ATetrominoPawn::MoveDown()
{
	MoveTo(ATetromino::DirectionDown);
}

void ATetrominoPawn::NormalFall()
{
	MoveDown();
}

void ATetrominoPawn::SetAutoRepeatMovement()
{
	static constexpr bool bIsAutoRepeatMovementLoop = true;
	GetWorldTimerManager().SetTimer(AutoRepeatMovementTimerHandle, this, &ATetrominoPawn::MoveToCurrentDirection, AutoRepeatMovementInterval, bIsAutoRepeatMovementLoop, AutoRepeatMovementInitialDelay);
}

void ATetrominoPawn::SetNormalFallTimer()
{
	if (GameMode && !GameMode->bNormalFallOff)
	{
		const float NormalFallSpeed = GameMode->GetFallSpeed();
		GetWorldTimerManager().SetTimer(NormalFallTimerHandle, this, &ATetrominoPawn::NormalFall, NormalFallSpeed, bIsNormalFallTimerLoop, NormalFallTimerInitialDelay);
	}
}

void ATetrominoPawn::SetMovementDirection(const FVector2D& NewMovementDirection)
{
	MovementDirection = NewMovementDirection;
}
