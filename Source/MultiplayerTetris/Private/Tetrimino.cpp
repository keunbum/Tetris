// Copyright Ryu KeunBeom. All Rights Reserved.

#include "Tetrimino.h"

#include "GhostPiece.h"
#include "Board.h"

ATetrimino::ATetrimino()
	: bIsGhostPieceOn(false)
	, GhostPiece(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATetrimino::SetGhostPiece(AGhostPiece* const InGhostPiece)
{
	GhostPiece = InGhostPiece;
	GhostPiece->Initialize(GetShape());
}

void ATetrimino::AttachToBoard(ABoard* const Board)
{
	AttachToComponentByMatrixLocation(Board->GetMatrixRoot(), GetInitialMatrixLocation());
}
