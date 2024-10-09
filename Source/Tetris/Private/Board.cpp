// Copyright Ryu KeunBeom. All Rights Reserved.


#include "Board.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "Algo/AllOf.h"
#include "TetriminoBase.h"

const FMinoInfo ABoard::BackgroundMinoInfo = FMinoInfo(TEXT("/Game/Material/M_MinoMaterial"), FLinearColor::Gray, 1.0f, 0);
const FMinoInfo ABoard::SpecialMinoInfo = FMinoInfo(TEXT("/Game/Material/M_MinoMaterial"), FLinearColor::Black, 1.0f, 0);

ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = false;

	MinoClass = UMino::StaticClass();

	// 음수/양수 순서대로 FVector(우/좌, 아래/위, 생략)
	MatrixRelativeLocation = FVector(MatrixVisibleWidth / 2, MatrixVisibleHeight, 0.f);
	NextQueueRelativeLocation = FVector(UMino::UnitLength * -12.f, UMino::UnitLength * -15.f, -UMino::UnitLength * 10);
	HoldQueueRelativeLocation = FVector(UMino::UnitLength * 6.f, UMino::UnitLength * -15.f, -UMino::UnitLength * 10);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	/** MatrixRoot */
	MatrixRoot = CreateAndSetupSceneComponent(TEXT("MatrixRoot"), RootComponent);
	if (MatrixRoot)
	{
		MatrixRoot->SetRelativeLocation(MatrixRelativeLocation);
	}

	/** BackgroundRoot */
	BackgroundRoot = CreateAndSetupSceneComponent(TEXT("BackgroundRoot"), MatrixRoot);

	/** NextQueueRoot */
	NextQueueRoot = CreateAndSetupSceneComponent(TEXT("NextQueueRoot"), MatrixRoot);
	if (NextQueueRoot)
	{
		NextQueueRoot->SetRelativeLocation(NextQueueRelativeLocation);
	}

	/** HoldQueueRoot */
	HoldQueueRoot = CreateAndSetupSceneComponent(TEXT("HoldQueueRoot"), MatrixRoot);
	if (HoldQueueRoot)
	{
		HoldQueueRoot->SetRelativeLocation(HoldQueueRelativeLocation);
	}
}

void ABoard::Initialize()
{
	//InitializeBackground();
	InitializeMinoMatrix();
}

bool ABoard::IsDirectlyAboveSurface(const ATetrimino* Tetrimino) const
{
	static const FIntPoint MovementDown = ATetriminoBase::GetMatrixMovementIntPointByDirection(ATetrimino::MoveDirectionDown);
	return !IsMovementPossible(Tetrimino, MovementDown);
}

bool ABoard::IsBlocked(const ATetrimino* Tetrimino) const
{
	return !IsMovementPossible(Tetrimino, FIntPoint::ZeroValue);
}

bool ABoard::IsAboveSkyline(const ATetrimino* Tetrimino) const
{
	if (Tetrimino)
	{
		const FIntPoint& TetriminoMatrixLocation = Tetrimino->GetMatrixLocation();
		const TArray<FIntPoint>& MinoTetriminoLocalLocations = Tetrimino->GetMinoTetriminoLocalLocations();
		return Algo::AllOf(MinoTetriminoLocalLocations, [this, &TetriminoMatrixLocation](const FIntPoint& MinoTetriminoLocalLocation) {
			const FIntPoint MinoLocalMatrixLocation = TetriminoMatrixLocation + MinoTetriminoLocalLocation;
			return MinoLocalMatrixLocation.X < SkyLine;
			}
		);
	}

	return false;
}

bool ABoard::IsMovementPossible(const ATetrimino* Tetrimino, const FIntPoint& MovementIntPoint2D) const
{
	if (Tetrimino)
	{
		const FIntPoint NewTetriminoMatrixLocation = Tetrimino->GetMatrixLocation() + MovementIntPoint2D;
		const TArray<FIntPoint>& MinoTetriminoLocalLocations = Tetrimino->GetMinoTetriminoLocalLocations();
		return IsMinoLocationsPossible(MinoTetriminoLocalLocations, NewTetriminoMatrixLocation);
	}

	return false;
}

bool ABoard::IsRotationPossible(const ATetrimino* Tetrimino, const ETetriminoRotationDirection RotationDirection, const FIntPoint& RotationPointOffset) const
{
	if (Tetrimino)
	{
		const FIntPoint& NewTetriminoMatrixLocation = Tetrimino->GetMatrixLocation() + RotationPointOffset;
		const ETetriminoShape TetriminoShape = Tetrimino->GetShape();
		const ETetriminoFacing NewTetriminoFacing = Tetrimino->GetFacing() + static_cast<int32>(RotationDirection);
		const TArray<FIntPoint>& NewMinoLocalMatrixLocations = ATetriminoBase::GetMinoTetriminoLocalLocationsByTetriminoShapeAndFacing(TetriminoShape, NewTetriminoFacing);
		return IsMinoLocationsPossible(NewMinoLocalMatrixLocations, NewTetriminoMatrixLocation);
	}

	return false;
}

bool ABoard::IsRowFull(const int32 TargetRow) const
{
	for (int32 TargetCol = VisibleBeginCol; TargetCol < VisibleEndCol; ++TargetCol)
	{
		const FIntPoint TargetMatrixLocation(TargetRow, TargetCol);
		if (IsMatrixLocationEmpty(TargetMatrixLocation))
		{
			return false;
		}
	}
	return true;
}

void ABoard::AddMinos(const ATetrimino* Tetrimino)
{
	if (Tetrimino)
	{
		const FIntPoint TetriminoMatrixLocation = Tetrimino->GetMatrixLocation();
		const TArray<FIntPoint>& MinoTetriminoLocalLocations = Tetrimino->GetMinoTetriminoLocalLocations();
		const TArray<UMino*>& MinoArray = Tetrimino->GetMinoArray();
		for (int32 MinoIndex = 0; MinoIndex < Tetrimino->MinoNum; ++MinoIndex)
		{
			const FIntPoint& MinoTetriminoLocalLocation = MinoTetriminoLocalLocations[MinoIndex];
			const FIntPoint MinoMatrixLocation = TetriminoMatrixLocation + MinoTetriminoLocalLocation;
			UMino* const Mino = MinoArray[MinoIndex];
			AddMino(Mino, MinoMatrixLocation);
		}
	}
}

void ABoard::ClearRows(const TArray<int32>& TargetRows)
{
	// Clear Rows
	for (const int32 TargetRow : TargetRows)
	{
		ClearRow(TargetRow);
	}

	// Move Rows
	for (int32 Index = TargetRows.Num() - 1; Index >= 0; --Index)
	{
		static constexpr int32 LastEndRow = TotalBeginRow - 1;
		const int32 EmptyRow = TargetRows[Index];
		// [BeginRow, EndRow)
		const int32 BeginRow = EmptyRow - 1;
		const int32 EndRow = (Index - 1 >= 0 ? TargetRows[Index - 1] : LastEndRow);
		for (int32 Row = BeginRow; Row > EndRow; --Row)
		{
			const int32 MoveDistance = TargetRows.Num() - Index;
			MoveRow(Row, MoveDistance);
		}
	}
}

FIntPoint ABoard::GetFinalFallingMatrixLocation(const ATetrimino* Tetrimino) const
{
	if (Tetrimino)
	{
		FIntPoint FinalFallingMatrixLocation = Tetrimino->GetMatrixLocation();
		static const FIntPoint MovementDown = ATetriminoBase::GetMatrixMovementIntPointByDirection(ATetrimino::MoveDirectionDown);
		while (IsMinoLocationsPossible(Tetrimino->GetMinoTetriminoLocalLocations(), FinalFallingMatrixLocation + MovementDown))
		{
			FinalFallingMatrixLocation += MovementDown;
		}
		return FinalFallingMatrixLocation;
	}

	return FIntPoint::ZeroValue;
}

void ABoard::InitializeBackground()
{
	for (int32 Row = VisibleBeginRow; Row < VisibleEndRow; ++Row)
	{
		const FMinoInfo& MinoInfo = BackgroundMinoInfo;
		for (int32 Col = VisibleBeginCol; Col < VisibleEndCol; ++Col)
		{
			const FIntPoint MinoMatrixLocation(Row, Col);
			static constexpr float Z = 0.f - UMino::UnitLength;
			if (UMino* const Mino = UMino::CreateMino(this, MinoInfo))
			{
				Mino->AttachToWithMatrixLocation(BackgroundRoot, MinoMatrixLocation, Z);
			}
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
	const UMino* Mino = GetMinoByMatrixLocation(MatrixLocation);
	return !IsValid(Mino);
}

bool ABoard::IsMinoLocationsPossible(const TArray<FIntPoint>& MinoTetriminoLocalLocations, const FIntPoint& TetriminoMatrixLocation) const
{
	return Algo::AllOf(MinoTetriminoLocalLocations, [this, &TetriminoMatrixLocation](const FIntPoint& MinoTetriminoLocalLocation) {
		const FIntPoint NewMinoLocalMatrixLocation = TetriminoMatrixLocation + MinoTetriminoLocalLocation;
		return FMath::IsWithin(NewMinoLocalMatrixLocation.X, TotalBeginRow, VisibleEndRow)
			&& FMath::IsWithin(NewMinoLocalMatrixLocation.Y, VisibleBeginCol, VisibleEndCol)
			&& (IsMatrixLocationEmpty(NewMinoLocalMatrixLocation));
		}
	);
}

USceneComponent* ABoard::CreateAndSetupSceneComponent(const FName& ComponentName, USceneComponent* const Parent)
{
	if (USceneComponent* const SceneComponent = CreateDefaultSubobject<USceneComponent>(ComponentName))
	{
		SceneComponent->SetupAttachment(Parent);
		return SceneComponent;
	}
	checkNoEntry();
	return nullptr;
}

void ABoard::AddMino(UMino* const Mino, const FIntPoint& MinoMatrixLocation)
{
	// Change ownership of the component to the board
	if (Mino)
	{
		Mino->Rename(nullptr, this);
		Mino->AttachToWithMatrixLocation(MatrixRoot, MinoMatrixLocation);
		SetMinoByMatrixLocation(Mino, MinoMatrixLocation);
	}
}

void ABoard::ClearRow(const int32 TargetRow)
{
	for (int32 TargetCol = VisibleBeginCol; TargetCol < VisibleEndCol; ++TargetCol)
	{
		const FIntPoint MinoMatrixLocation(TargetRow, TargetCol);
		UMino* const Mino = GetMinoByMatrixLocation(MinoMatrixLocation);
		RemoveMino(Mino, MinoMatrixLocation);
	}
}

void ABoard::MoveRow(const int32 TargetRow, const int32 MoveDistance)
{
	for (int32 TargetCol = VisibleBeginCol; TargetCol < VisibleEndCol; ++TargetCol)
	{
		const FIntPoint MinoMatrixLocation(TargetRow, TargetCol);
		if (!IsMatrixLocationEmpty(MinoMatrixLocation))
		{
			UMino* const Mino = GetMinoByMatrixLocation(MinoMatrixLocation);
			const FIntPoint NewMinoMatrixLocation(/* NewRow */ TargetRow + MoveDistance, TargetCol);
			MoveMino(Mino, /* OldMatrixLocation */ MinoMatrixLocation, NewMinoMatrixLocation);
		}
	}
}

void ABoard::RemoveMino(UMino* const Mino, const FIntPoint& MinoMatrixLocation)
{
	if (Mino)
	{
		Mino->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Mino->DestroyComponent();
		SetMinoByMatrixLocation(nullptr, MinoMatrixLocation);
	}
}

void ABoard::MoveMino(UMino* const Mino, const FIntPoint& OldMatrixLocation, const FIntPoint& NewMatrixLocation)
{
	if (Mino)
	{
		Mino->SetRelativeLocationByMatrixLocation(NewMatrixLocation);
		SetMinoByMatrixLocation(Mino, NewMatrixLocation);
		SetMinoByMatrixLocation(nullptr, OldMatrixLocation);
	}
}
