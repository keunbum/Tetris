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
	check(InGhostPiece != nullptr);
	GhostPiece = InGhostPiece;
	GhostPiece->Initialize(Shape, Facing);
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
	SetFacing(Facing + static_cast<int32>(RotationDirection));
	UpdateMinoMatrixLocalLocations();
}

void ATetrimino::AttachToBoard(ABoard* const InBoard)
{
	check(InBoard != nullptr);
	Board = InBoard;
	AttachToComponentByMatrixLocation(Board->GetMatrixRoot(), GetInitialMatrixLocation());
}
