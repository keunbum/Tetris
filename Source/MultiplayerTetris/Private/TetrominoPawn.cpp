// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "TetrominoPawn.h"
#include "EnhancedInputComponent.h"

#include "Board.h"
#include "TetrisGameModeBase.h"
#include "Tetrimino.h"

// Sets default values
ATetrominoPawn::ATetrominoPawn()
	: NormalFallSpeed(-1.0f)
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
	if (UEnhancedInputComponent* const EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Pawn에 직접 바인딩할 것 있으면 추가.
	}
}

void ATetrominoPawn::StartMovement(const FVector2D& InMovementDirection)
{
	SetMovementDirection(InMovementDirection);
	MoveToCurrentDirection();
	SetAutoRepeatMovement();
}

void ATetrominoPawn::EndMovement()
{
	ClearTimer(AutoRepeatMovementTimerHandle);
	SetMovementDirection(FVector2D::ZeroVector);
}

void ATetrominoPawn::StartSoftDrop()
{
	if (TetrominoInPlay)
	{
		// NormalFall 일시 중지
		ClearTimer(NormalFallTimerHandle);

		GetWorldTimerManager().SetTimer(SoftDropTimerHandle, this, &ATetrominoPawn::MoveDown, GameMode->GetSoftDropSpeed(), bSoftDropTimerLoop, SoftDropTimerInitialDelay);
	}
}

void ATetrominoPawn::EndSoftDrop()
{
	ClearTimer(SoftDropTimerHandle);

	// NormalFall 재개
	SetNormalFallTimer();
}

void ATetrominoPawn::StartHardDrop()
{
	// TODO: 하드 드롭 로직 추가
}

void ATetrominoPawn::StartRotate(const int32 RotationDirection)
{
	if (TetrominoInPlay)
	{
		TetrominoInPlay->RotateTo(RotationDirection);
	}
}

void ATetrominoPawn::Initialize()
{
	GameMode = GetWorld()->GetAuthGameMode<ATetrisGameModeBase>();
	SetNormalFallTimer();
	SetMovementDirection(FVector2D::ZeroVector);
}

void ATetrominoPawn::ClearTimer(FTimerHandle& InOutTimerHandle)
{
	GetWorldTimerManager().ClearTimer(InOutTimerHandle);
}

void ATetrominoPawn::MoveTo(const FVector2D& Direction)
{
	if (TetrominoInPlay)
	{
		const bool bIsNextPositionPossible = true;
		if (bIsNextPositionPossible)
		{
			TetrominoInPlay->Move(Direction);
		}
	}
}

void ATetrominoPawn::MoveToCurrentDirection()
{
	MoveTo(MovementDirection);
}

void ATetrominoPawn::MoveDown()
{
	MoveTo(ATetrimino::MoveDirectionDown);
}

void ATetrominoPawn::SetAutoRepeatMovement()
{
	GetWorldTimerManager().SetTimer(AutoRepeatMovementTimerHandle, this, &ATetrominoPawn::MoveToCurrentDirection, AutoRepeatMovementInterval, bIsAutoRepeatMovementLoop, AutoRepeatMovementInitialDelay);
}

void ATetrominoPawn::SetNormalFallTimer()
{
	if (GameMode && !GameMode->bNormalFallOff)
	{
		GetWorldTimerManager().SetTimer(NormalFallTimerHandle, this, &ATetrominoPawn::MoveDown, NormalFallSpeed, bIsNormalFallTimerLoop, NormalFallTimerInitialDelay);
	}
}

