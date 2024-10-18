// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetriminoGhostPiece.h"

#include "Mino.h"

const FMinoInfo ATetriminoGhostPiece::GetMinoInfo() const
{
	const FTetriminoShapeInfo& ShapeInfo = GetTetriminoShapeInfo();
	return FMinoInfo(UMino::DefaultMaterialPath, ShapeInfo.BaseColor, ATetriminoGhostPiece::Opacity, ATetriminoGhostPiece::TranslucentSortPriority);
}

void ATetriminoGhostPiece::AttachToMatrix(USceneComponent* const MatrixRoot)
{
	if (RootComponent)
	{
		RootComponent->AttachToComponent(MatrixRoot, FAttachmentTransformRules::KeepRelativeTransform);
	}
}
