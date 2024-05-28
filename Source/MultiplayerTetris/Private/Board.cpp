// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "Board.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

#include "Mino.h"
#include "Tetromino.h"
#include "TetrominoPawn.h"

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TetrominoInPlay = nullptr;
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();

	{
		const FVector SpawnLocation(FVector::ZeroVector);
		const FRotator SpawnRotation(FRotator::ZeroRotator);
		const ETetrominoType TetrominoType(ETetrominoType::I);
		SpawnTetromino(SpawnLocation, SpawnRotation, TetrominoType);
	}
}

// Called every frame
void ABoard::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::AttachTetromino(ATetromino* const NewTetrominoInPlay)
{
	if (NewTetrominoInPlay)
	{
		NewTetrominoInPlay->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		NewTetrominoInPlay->Initialize();

		// TetrominoPawn¿¡ TetrominoInPlay ¼³Á¤
		if (APlayerController* const PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (ATetrominoPawn* const TetrominoPawn = Cast<ATetrominoPawn>(PlayerController->GetPawn()))
			{
				TetrominoPawn->SetTetrominoInPlay(NewTetrominoInPlay);
			}
		}

		TetrominoInPlay = NewTetrominoInPlay;
	}
}

void ABoard::Initialize()
{
}

void ABoard::SpawnTetromino(const FVector& SpawnLocation, const FRotator& SpawnRotation, const ETetrominoType TetrominoType)
{
	TetrominoInPlay = GetWorld()->SpawnActor<ATetromino>(TetrominoClass, SpawnLocation, SpawnRotation);
	ensureMsgf(TetrominoInPlay != nullptr, TEXT("FUCK. TetrominoInPlay is null"));
	if (TetrominoInPlay)
	{
		TetrominoInPlay->SetTetrominoType(TetrominoType);
		AttachTetromino(TetrominoInPlay);
	}
}

void ABoard::SpawnTetrominos()
{
	FVector SpawnLocation = FVector::ZeroVector;
	for (int32 Index = static_cast<int32>(ETetrominoType::O); Index <= static_cast<int32>(ETetrominoType::Z); ++Index)
	{
		ETetrominoType TetrominoType = static_cast<ETetrominoType>(Index);
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		SpawnTetromino(SpawnLocation, SpawnRotation, TetrominoType);
		SpawnLocation.Y -= 2 * AMino::UnitLength;
	}
}
