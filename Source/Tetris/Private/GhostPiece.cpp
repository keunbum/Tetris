// Copyright Ryu KeunBeom. All Rights Reserved.

#include "GhostPiece.h"

#include "Mino.h"

const FMinoInfo AGhostPiece::GetMinoInfo() const
{
	const FTetriminoShapeInfo& ShapeInfo = GetTetriminoShapeInfo();
	return FMinoInfo(ShapeInfo.MaterialPath, ShapeInfo.Color, AGhostPiece::Opacity, AGhostPiece::TranslucentSortPriority);
}

void AGhostPiece::AttachToMatrix(USceneComponent* const MatrixRoot)
{
	if (RootComponent && MatrixRoot)
	{
		RootComponent->AttachToComponent(MatrixRoot, FAttachmentTransformRules::KeepRelativeTransform);
	}
}
