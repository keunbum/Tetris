// Fill out your copyright notice in the Description page of Project Settings.


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
void ATetrominoPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATetrominoPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATetrominoPawn::SetTetrominoInPlay(ATetromino* const NewTetrominoInPlay)
{
	TetrominoInPlay = NewTetrominoInPlay;
}

void ATetrominoPawn::OnMoveLeft()
{
	if (TetrominoInPlay)
	{
		TetrominoInPlay->Move(ATetromino::DirectionLeft);
	}
}

void ATetrominoPawn::OnMoveRight()
{
	if (TetrominoInPlay)
	{
		TetrominoInPlay->Move(ATetromino::DirectionRight);
	}
}

void ATetrominoPawn::OnSoftDrop()
{
	if (TetrominoInPlay && GameMode)
	{
		// 소프트 드롭 타이머 설정 (반복 실행되도록 설정)
		const float SoftDropSpeed = GameMode->GetSoftDropSpeed();
		SetFallTimer(SoftDropTimerHandle, SoftDropSpeed, bSoftDropTimerLoop, SoftDropTimerFirstDelayTime);
	}
}

void ATetrominoPawn::OnStopSoftDrop()
{
	ClearTimer(SoftDropTimerHandle);
}

void ATetrominoPawn::OnHardDrop()
{
	// TODO: 하드 드롭 로직 추가
}

void ATetrominoPawn::UpdateNormalFallSpeed(const float NewFallSpeed)
{
	ClearTimer(NormalFallTimerHandle);
	SetFallTimer(NormalFallTimerHandle, NewFallSpeed, bIsNormalFallTimerLoop, NormalFallTimerFirstDelayTime);
}

void ATetrominoPawn::Initialize()
{
	GameMode = GetWorld()->GetAuthGameMode<ATetrisGameModeBase>();
	if (GameMode)
	{
		SetInitialTimers();
	}
}

void ATetrominoPawn::SetInitialTimers()
{
	if (!GameMode->bNormalFallOff)
	{
		// 기본 낙하 타이머 설정
		const float NormalFallSpeed = GameMode->GetFallSpeed();
		SetFallTimer(NormalFallTimerHandle, NormalFallSpeed, bIsNormalFallTimerLoop, NormalFallTimerFirstDelayTime);
	}
}

void ATetrominoPawn::ClearTimer(FTimerHandle& InOutTimerHandle)
{
	GetWorldTimerManager().ClearTimer(InOutTimerHandle);
}

void ATetrominoPawn::SetFallTimer(FTimerHandle& InOutFallTimerHandle, const float NewFallSpeed, const bool bIsTimerLoop, const float FirstDelayTime)
{
	GetWorldTimerManager().SetTimer(InOutFallTimerHandle, this, &ATetrominoPawn::OnFallTimer, NewFallSpeed, bIsTimerLoop, FirstDelayTime);
}

void ATetrominoPawn::OnFallTimer()
{
	if (TetrominoInPlay)
	{
		TetrominoInPlay->Move(ATetromino::DirectionDown);
	}
}

