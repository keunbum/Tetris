// Copyright Ryu KeunBeom. All Rights Reserved.

#include "Tetrimino.h"

#include "GhostPiece.h"
#include "Board.h"

ATetrimino::ATetrimino()
	: GhostPiece(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATetrimino::SetGhostPiece(AGhostPiece* const InGhostPiece)
{
	GhostPiece = InGhostPiece;
	if (GhostPiece)
	{
		GhostPiece->Initialize(Shape, Facing);
		check(Board != nullptr);
		GhostPiece->SetRelativeLocationByMatrixLocation(Board->GetFinalFallingMatrixLocation(this));
	}
}

void ATetrimino::MoveBy(const FIntPoint& IntPoint2D)
{
	AddRelativeLocationByMatrixLocationOffset(IntPoint2D);

	if (GhostPiece)
	{
		check(Board != nullptr);
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

void ATetrimino::SetBoard(ABoard* const InBoard)
{
	Board = InBoard;
	if (Board)
	{
		AttachToComponentByMatrixLocation(Board->GetMatrixRoot(), GetInitialMatrixLocation());
	}
}
