// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "Board.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "Algo/AllOf.h"

#include "Tetrimino.h"

const FMinoInfo ABoard::BackgroundMinoInfo = FMinoInfo(TEXT("/Game/Material/M_MinoMaterial"), FLinearColor::Gray);
const FMinoInfo ABoard::SpecialMinoInfo = FMinoInfo(TEXT("/Game/Material/M_MinoMaterial"), FLinearColor::Black);

ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MinoClass = UMino::StaticClass();

	BackgroundRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BackgroundRoot"));
	BackgroundRoot->SetupAttachment(RootComponent);
}

void ABoard::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABoard::IsMovementPossible(const ATetrimino* Tetrimino, const FIntPoint& MovementIntPoint2D) const
{
	check(Tetrimino != nullptr);
	const FIntPoint NewTetriminoMatrixLocation = Tetrimino->GetMatrixLocation() + MovementIntPoint2D;
	const TArray<FIntPoint>& MinoLocalMatrixLocations = Tetrimino->GetMinoMatrixLocalLocations();
	return IsMinoLocationsPossible(NewTetriminoMatrixLocation, MinoLocalMatrixLocations);
}

bool ABoard::IsRotationPossible(const ATetrimino* Tetrimino, const ETetriminoRotationDirection RotationDirection, const FIntPoint& RotationPointOffset) const
{
	check(Tetrimino != nullptr);
	const FIntPoint& NewTetriminoMatrixLocation = Tetrimino->GetMatrixLocation() + RotationPointOffset;
	const ETetriminoShape TetriminoShape = Tetrimino->GetShape();
	const ETetriminoFacing NewTetriminoFacing = Tetrimino->GetFacing() + static_cast<int32>(RotationDirection);
	const TArray<FIntPoint>& NewMinoLocalMatrixLocations = ATetrimino::GetMinoMatrixLocalLocationsByTetriminoShapeAndFacing(TetriminoShape, NewTetriminoFacing);
	return IsMinoLocationsPossible(NewTetriminoMatrixLocation, NewMinoLocalMatrixLocations);
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ABoard::Initialize()
{
	InitializeBackground();
	InitializeMinoMatrix();
}

void ABoard::InitializeBackground()
{
	for (int32 Row = 0; Row < TotalHeight; ++Row)
	{
		const FMinoInfo& MinoInfo = (Row == (TotalHeight - VisibleHeight) ? SpecialMinoInfo : BackgroundMinoInfo);
		for (int32 Col = 0; Col < TotalWidth; ++Col)
		{
			const FIntPoint MinoMatrixLocation(Row, Col);
			static constexpr float Z = 0 - UMino::UnitLength;
			UMino* const Mino = UMino::CreateMino(this, BackgroundRoot, MinoInfo, MinoMatrixLocation, Z);
			check(Mino != nullptr);
		}
	}
}

void ABoard::InitializeMinoMatrix()
{
	MinoMatrix.Reserve(TotalHeight * TotalWidth);
	for (int32 _ = 0; _ < TotalHeight * TotalWidth; ++_)
	{
		UMino* const Mino = nullptr;
		MinoMatrix.Add(Mino);
	}
}

UMino* ABoard::GetMinoByMatrixLocation(const FIntPoint& MatrixLocation) const
{
	const int32 Index = TotalWidth * MatrixLocation.X + MatrixLocation.Y;
	return MinoMatrix[Index];
}

bool ABoard::IsMatrixLocationEmpty(const FIntPoint& MatrixLocation) const
{
	return (GetMinoByMatrixLocation(MatrixLocation) == nullptr);
}

bool ABoard::IsMinoLocationsPossible(const FIntPoint& TetriminoMatrixLocation, const TArray<FIntPoint>& MinoLocalMatrixLocations) const
{
	return Algo::AllOf(MinoLocalMatrixLocations, [this, &TetriminoMatrixLocation](const FIntPoint& MinoLocalMatrixLocation) {
		const FIntPoint NewMinoLocalMatrixLocation = TetriminoMatrixLocation + MinoLocalMatrixLocation;
		return FMath::IsWithin(NewMinoLocalMatrixLocation.X, TotalBeginRow, VisibleEndRow)
			&& FMath::IsWithin(NewMinoLocalMatrixLocation.Y, VisibleBeginCol, VisibleEndCol)
			&& (IsMatrixLocationEmpty(NewMinoLocalMatrixLocation));
		}
	);
}
