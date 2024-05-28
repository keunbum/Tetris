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

	// Create and set the default root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Initialize MinoClass and TetrominoClass with StaticClass method
	MinoClass = AMino::StaticClass();
	TetrominoClass = ATetromino::StaticClass();

	TetrominoInPlay = nullptr;
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();

	TestTetrominoSpawning();
}

// Called every frame
void ABoard::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::AttachTetromino(ATetromino* const NewTetromino)
{
	if (NewTetromino)
	{
		TetrominoInPlay = NewTetromino;
		TetrominoInPlay->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		// TetrominoPawn에 TetrominoInPlay 설정
		if (APlayerController* const PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (ATetrominoPawn* const TetrominoPawn = Cast<ATetrominoPawn>(PlayerController->GetPawn()))
			{
				TetrominoPawn->SetTetrominoInPlay(TetrominoInPlay);
			}
		}
	}
}

void ABoard::Initialize()
{
}

void ABoard::SpawnTetromino(const FVector& SpawnLocation, const FRotator& SpawnRotation, const ETetrominoType TetrominoType)
{
	TetrominoInPlay = GetWorld()->SpawnActor<ATetromino>(TetrominoClass, SpawnLocation, SpawnRotation);
	if (TetrominoInPlay)
	{
		TetrominoInPlay->SetTetrominoType(TetrominoType);
		TetrominoInPlay->Initialize();
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

void ABoard::TestTetrominoSpawning()
{
	const FVector SpawnLocation(FVector::ZeroVector);
	const FRotator SpawnRotation(FRotator::ZeroRotator);
	const ETetrominoType TetrominoType(ETetrominoType::I);
	SpawnTetromino(SpawnLocation, SpawnRotation, TetrominoType);
}
