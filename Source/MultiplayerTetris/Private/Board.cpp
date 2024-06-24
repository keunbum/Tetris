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
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MinoClass = UMino::StaticClass();

	BackgroundRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BackgroundRoot"));
	BackgroundRoot->SetupAttachment(RootComponent);
}

void ABoard::Initialize()
{
	InitializeBackground();
	InitializeMinoMatrix();
}

bool ABoard::IsMovementPossible(const ATetrimino* Tetrimino, const FIntPoint& MovementIntPoint2D) const
{
	check(Tetrimino != nullptr);
	const FIntPoint NewTetriminoMatrixLocation = Tetrimino->GetMatrixLocation() + MovementIntPoint2D;
	const TArray<FIntPoint>& MinoMatrixLocalLocations = Tetrimino->GetMinoMatrixLocalLocations();
	return IsMinoLocationsPossible(NewTetriminoMatrixLocation, MinoMatrixLocalLocations);
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

void ABoard::AddMinos(const ATetrimino* Tetrimino)
{
	check(Tetrimino != nullptr);

	const FIntPoint TetriminoMatrixLocation = Tetrimino->GetMatrixLocation();
	const TArray<FIntPoint>& MinoMatrixLocalLocations = Tetrimino->GetMinoMatrixLocalLocations();
	const TArray<UMino*>& MinoArray = Tetrimino->GetMinoArray();
	for (int32 MinoIndex = 0; MinoIndex < Tetrimino->MinoNum; ++MinoIndex)
	{
		const FIntPoint& MinoMatrixLocalLocation = MinoMatrixLocalLocations[MinoIndex];
		const FIntPoint MinoMatrixLocation = TetriminoMatrixLocation + MinoMatrixLocalLocation;
		UMino* const Mino = MinoArray[MinoIndex];
		// Change ownership of the component to the board
		Mino->Rename(nullptr, this);
		Mino->AttachToWithMatrixLocation(BackgroundRoot, MinoMatrixLocation);
		SetMinoByMatrixLocation(Mino, MinoMatrixLocation);
	}
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
			UMino* const Mino = UMino::CreateMino(this, MinoInfo);
			check(Mino != nullptr);
			Mino->AttachToWithMatrixLocation(BackgroundRoot, MinoMatrixLocation, Z);
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

int32 ABoard::GetMatrixIndexByMatrixLocation(const FIntPoint& MatrixLocation) const
{
	return TotalWidth * MatrixLocation.X + MatrixLocation.Y;
}

UMino* ABoard::GetMinoByMatrixLocation(const FIntPoint& MatrixLocation) const
{
	const int32 Index = GetMatrixIndexByMatrixLocation(MatrixLocation);
	return MinoMatrix[Index];
}

void ABoard::SetMinoByMatrixLocation(UMino* const Mino, const FIntPoint& MatrixLocation)
{
	const int32 Index = GetMatrixIndexByMatrixLocation(MatrixLocation);
	MinoMatrix[Index] = Mino;
}

bool ABoard::IsMatrixLocationEmpty(const FIntPoint& MatrixLocation) const
{
	UMino* const Mino = GetMinoByMatrixLocation(MatrixLocation);
	const bool bIsMinoValid = (Mino != nullptr) && !Mino->IsPendingKill();
	return !bIsMinoValid;
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
