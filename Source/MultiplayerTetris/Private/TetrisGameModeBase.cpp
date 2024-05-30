// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "TetrisGameModeBase.h"

#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "TetrominoPawn.h"

ATetrisGameModeBase::ATetrisGameModeBase()
	: CurrentLevel(DefaultGameLevel)
{
}

void ATetrisGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ATetrisGameModeBase::Initialize()
{
}

int32 ATetrisGameModeBase::GetCurrentLevel() const
{
	return CurrentLevel;
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

float ATetrisGameModeBase::GetFallSpeed() const
{
	return CalculateFallSpeed(CurrentLevel);
}

float ATetrisGameModeBase::CalculateFallSpeed(const int32 Level)
{
	const float A = 0.8f - ((Level - 1) * 0.007f);
	const float B = static_cast<float>(Level - 1);
	return FMath::Pow(A, B);
}

