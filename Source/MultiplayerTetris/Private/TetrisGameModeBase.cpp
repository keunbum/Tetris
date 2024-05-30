// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "TetrisGameModeBase.h"

#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "Tetrimino.h"
#include "TetrominoPawn.h"
#include "TetrisMatrix.h"

ATetrisGameModeBase::ATetrisGameModeBase()
	: CurrentLevel(DefaultGameLevel)
{

	TetriminoClass = ATetrimino::StaticClass();
}

void ATetrisGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
	StartGenerationPhase();
}

void ATetrisGameModeBase::UpCurrentLevel()
{
	CurrentLevel += 1;

	if (APawn* const PlayerPawn = UGameplayStatics::GetPlayerPawn(this, PlayerIndex);
		ATetrominoPawn* const TetrominoPawn = Cast<ATetrominoPawn>(PlayerPawn))
	{
		const float NewFallSpeed = GetFallSpeed();
		TetrominoPawn->SetNormalFallSpeed(NewFallSpeed);
	}
}

void ATetrisGameModeBase::Initialize()
{
	Matrix = GetWorld()->SpawnActor<ATetrisMatrix>();
}

void ATetrisGameModeBase::StartGenerationPhase()
{
	TetriminoInPlay = SpawnNextTetrimino();
	check(TetriminoInPlay != nullptr);
}

ATetrimino* ATetrisGameModeBase::SpawnNextTetrimino() const
{
	if (ATetrimino* const NewTetrimino = GetWorld()->SpawnActor<ATetrimino>(TetriminoClass))
	{
		const ETetriminoType NewTetriminoType = ATetrimino::GetTetriminoTypeRandom();
		NewTetrimino->Initialize(NewTetriminoType);

		return NewTetrimino;
	}

	return nullptr;
}

float ATetrisGameModeBase::CalculateFallSpeed(const int32 Level)
{
	const float A = 0.8f - ((Level - 1) * 0.007f);
	const float B = static_cast<float>(Level - 1);
	return FMath::Pow(A, B);
}

