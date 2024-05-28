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
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrominoPawn::OnMoveLeft()!"));
	if (TetrominoInPlay)
	{
		TetrominoInPlay->Move(ATetromino::DirectionLeft);
	}
}

void ATetrominoPawn::OnMoveRight()
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrominoPawn::OnMoveRight()!"));
	if (TetrominoInPlay)
	{
		TetrominoInPlay->Move(ATetromino::DirectionRight);
	}
}

void ATetrominoPawn::OnSoftDrop()
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrominoPawn::OnSoftDrop()!"));
	if (TetrominoInPlay && GameMode)
	{
		// 소프트 드롭 타이머 설정 (반복 실행되도록 설정)
		const float SoftDropSpeed = GameMode->GetSoftDropSpeed();
		GetWorld()->GetTimerManager().SetTimer(SoftDropTimerHandle, this, &ATetrominoPawn::OnFallTimer, SoftDropSpeed, bSoftDropTimerLoop);
	}
}

void ATetrominoPawn::OnStopSoftDrop()
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrominoPawn::OnStopSoftDrop()!"));
	// 소프트 드롭 타이머 해제
	GetWorld()->GetTimerManager().ClearTimer(SoftDropTimerHandle);
}

void ATetrominoPawn::OnHardDrop()
{
	// TODO: 하드 드롭 로직 추가
}

void ATetrominoPawn::UpdateFallSpeed(const float NewFallSpeed)
{
	GetWorld()->GetTimerManager().ClearTimer(FallTimerHandle);
	SetFallTimer(NewFallSpeed);
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
		const float InitialFallSpeed = GameMode->GetFallSpeed();
		SetFallTimer(InitialFallSpeed);
	}
}

void ATetrominoPawn::SetFallTimer(const float NewFallSpeed)
{
	GetWorld()->GetTimerManager().SetTimer(FallTimerHandle, this, &ATetrominoPawn::OnFallTimer, NewFallSpeed, bIsFallTimerLoop, FallTimerFirstDelayTime);
}

void ATetrominoPawn::OnFallTimer()
{
	if (TetrominoInPlay)
	{
		TetrominoInPlay->Move(ATetromino::DirectionDown);
	}
}

