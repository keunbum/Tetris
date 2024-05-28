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

void ATetrominoPawn::MoveLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrominoPawn::MoveLeft()!"));
	if (TetrominoInPlay)
	{
		TetrominoInPlay->Move(ATetromino::DirectionLeft);
	}
}

void ATetrominoPawn::MoveRight()
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrominoPawn::MoveRight()!"));
	if (TetrominoInPlay)
	{
		TetrominoInPlay->Move(ATetromino::DirectionRight);
	}
}

void ATetrominoPawn::SoftDrop()
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrominoPawn::SoftDrop()!"));
	if (TetrominoInPlay)
	{
		TetrominoInPlay->Move(ATetromino::DirectionDown);
	}
}

void ATetrominoPawn::HardDrop()
{
	// TODO: 하드 드롭 로직 추가
}

void ATetrominoPawn::UpdateFallSpeed(const float NewFallSpeed)
{
	GetWorld()->GetTimerManager().ClearTimer(FallTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(FallTimerHandle, this, &ATetrominoPawn::OnFallTimer, NewFallSpeed, true);
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
	if (!GameMode->bTetrominoFallOff)
	{
		// 기본 낙하 타이머 설정
		const float FallSpeed = GameMode->GetFallSpeed();
		GetWorld()->GetTimerManager().SetTimer(FallTimerHandle, this, &ATetrominoPawn::OnFallTimer, FallSpeed, bIsFallTimerLoop, FallTimerFirstDelayTime);
	}
}

void ATetrominoPawn::OnFallTimer()
{
	if (TetrominoInPlay)
	{
		TetrominoInPlay->Move(ATetromino::DirectionDown);
	}
}

