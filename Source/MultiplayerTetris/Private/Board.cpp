// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "Board.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

#include "Mino.h"
#include "Tetrimino.h"
#include "TetrisPlayManager.h"
#include "TetrisGameModeBase.h"

const FString ABoard::BackgroundMinoMaterialPath = TEXT("/Game/Material/M_MinoMaterial_Grey");

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set the default root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Initialize Class variables
	MinoClass = AMino::StaticClass();
	TetrominoClass = ATetrimino::StaticClass();

	// Etc
	TetrominoInPlay = nullptr;
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();
	//TestTetrominoSpawning();
}

// Called every frame
void ABoard::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::AttachTetromino(ATetrimino* const NewTetromino)
{
	check(NewTetromino != nullptr);
	TetrominoInPlay = NewTetromino;
	TetrominoInPlay->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void ABoard::Initialize()
{
	InitializeBackground();
}

void ABoard::InitializeBackground()
{
	UMaterialInterface* const BackgroundMinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *BackgroundMinoMaterialPath));
	if (!BackgroundMinoMaterial)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load material: %s"), *BackgroundMinoMaterialPath);
		return;
	}

	Background.Reserve(TotalHeight * TotalWidth);
	for (int32 Row = 0; Row < TotalHeight; ++Row)
	{
		for (int32 Col = 0; Col < TotalWidth; ++Col)
		{
			if (AMino* const Mino = GetWorld()->SpawnActor<AMino>(MinoClass, FVector::ZeroVector, FRotator::ZeroRotator))
			{
				Mino->SetMaterial(BackgroundMinoMaterial);

				Mino->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				const FVector2D MinoUnitPosition(Row, Col);
				Mino->SetRelativeLocationByUnitVector2D(MinoUnitPosition);

				Background.Add(Mino);
			}
		}
	}
}

void ABoard::SpawnTetromino(const FVector& SpawnLocation, const FRotator& SpawnRotation, const ETetriminoType TetriminoType)
{
	TetrominoInPlay = GetWorld()->SpawnActor<ATetrimino>(TetrominoClass, SpawnLocation, SpawnRotation);
	if (TetrominoInPlay)
	{
		TetrominoInPlay->Initialize(TetriminoType);
		AttachTetromino(TetrominoInPlay);
	}
}

void ABoard::TestTetrominosSpawning()
{
	FVector SpawnLocation = FVector::ZeroVector;
	for (int32 Index = static_cast<int32>(ETetriminoType::O); Index <= static_cast<int32>(ETetriminoType::Z); ++Index)
	{
		ETetriminoType TetriminoType = static_cast<ETetriminoType>(Index);
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		SpawnTetromino(SpawnLocation, SpawnRotation, TetriminoType);
		SpawnLocation.Y -= 2 * AMino::UnitLength;
	}
}

void ABoard::TestTetrominoSpawning()
{
	const FVector SpawnLocation(FVector::ZeroVector);
	const FRotator SpawnRotation(FRotator::ZeroRotator);
	const ETetriminoType TetriminoType(TestType);
	SpawnTetromino(SpawnLocation, SpawnRotation, TetriminoType);
}
