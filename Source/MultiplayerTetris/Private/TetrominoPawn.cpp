// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrominoPawn.h"

#include "Tetromino.h"
#include "Board.h"

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
	ensureMsgf(TetrominoInPlay != nullptr, TEXT("FUCK. TetrominoInPlay is nullptr"));
}

void ATetrominoPawn::MoveLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrominoPawn::MoveLeft()!"));
	TetrominoInPlay->Move(ATetromino::DirectionLeft);
}

void ATetrominoPawn::MoveRight()
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrominoPawn::MoveRight()!"));
	TetrominoInPlay->Move(ATetromino::DirectionRight);
}

void ATetrominoPawn::SoftDrop()
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrominoPawn::SoftDrop()!"));
	TetrominoInPlay->Move(ATetromino::DirectionDown);
}

void ATetrominoPawn::HardDrop()
{
}

