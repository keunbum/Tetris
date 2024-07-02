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
	GhostPiece->Initialize(Shape);
}

void ATetrimino::AttachToBoard(ABoard* const Board)
{
	AttachToComponentByMatrixLocation(Board->GetMatrixRoot(), GetInitialMatrixLocation());
}
