// Copyright Ryu KeunBeom. All Rights Reserved.

#include "Tetrimino.h"

#include "GhostPiece.h"
#include "Board.h"
#include "Mino.h"
#include "Algo/MinElement.h"

ATetrimino::ATetrimino()
	: GhostPiece(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

const FMinoInfo ATetrimino::GetMinoInfo() const
{
	const FTetriminoShapeInfo& ShapeInfo = GetTetriminoShapeInfo();
	return FMinoInfo(UMino::DefaultMaterialPath, ShapeInfo.BaseColor, ATetrimino::Opacity, ATetrimino::TranslucentSortPriority);
}

void ATetrimino::SetGhostPiece(AGhostPiece* const InGhostPiece)
{
	GhostPiece = InGhostPiece;
	if (GhostPiece && Board)
	{
		const ATetriminoBase::FInitializeParams InitializeParams(Shape, Facing);
		GhostPiece->Initialize(InitializeParams);
		GhostPiece->SetRelativeLocationByMatrixLocation(Board->GetFinalFallingMatrixLocation(this));
		// GhostPiece를 화면에 보이게 한다 (ATetrisPlayManager::HardDrop 때문에 필요)
		GhostPiece->SetActorHiddenInGame(false);
	}
}

void ATetrimino::SetBoard(ABoard* const InBoard)
{
	Board = InBoard;
	if (Board)
	{
		AttachToMatrix(Board->GetMatrixRoot());
	}
}

int32 ATetrimino::GetLowestRow() const
{
	const TArray<FIntPoint>& MinoLocalLocations = GetMinoTetriminoLocalLocations();
	// X 값이 클수록 미노가 더 아래에 위치함.
	const FIntPoint LowestMinoLocalLocation = *Algo::MinElement(MinoLocalLocations, [](const FIntPoint& A, const FIntPoint& B) { return A.X > B.X; });
	return MatrixLocation.X + LowestMinoLocalLocation.X;
}

void ATetrimino::DetachFromBoard()
{
	GhostPiece = nullptr;

	if (Board)
	{
		DetachFromMatrix();
		Board = nullptr;
	}
}

void ATetrimino::MoveBy(const FIntPoint& IntPoint2D)
{
	AddRelativeLocationByMatrixLocationOffset(IntPoint2D);

	if (GhostPiece && Board)
	{
		GhostPiece->SetRelativeLocationByMatrixLocation(Board->GetFinalFallingMatrixLocation(this));
	}
}

void ATetrimino::RotateTo(const ETetriminoRotationDirection RotationDirection)
{
	const ETetriminoFacing NewFacing = Facing + static_cast<int32>(RotationDirection);

	RotateByFacing(NewFacing);
	if (GhostPiece)
	{
		GhostPiece->RotateByFacing(NewFacing);
	}
}

void ATetrimino::RotateToWithPointOffset(const ETetriminoRotationDirection RotationDirection, const FIntPoint& PointOffset)
{
	RotateTo(RotationDirection);
	MoveBy(PointOffset);
}

void ATetrimino::AttachToMatrix(USceneComponent* const MatrixRoot)
{
	AttachToComponentByMatrixLocation(MatrixRoot, GetInitialMatrixLocation());
}

void ATetrimino::DetachFromMatrix()
{
	if (RootComponent)
	{
		RootComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	}
}
