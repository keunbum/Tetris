// Copyright Ryu KeunBeom. All Rights Reserved.

#include "GhostPiece.h"

#include "Mino.h"

const FMinoInfo AGhostPiece::GetMinoInfo() const
{
	const FTetriminoShapeInfo& ShapeInfo = GetTetriminoShapeInfo();
	return FMinoInfo(UMino::DefaultMaterialPath, ShapeInfo.BaseColor, AGhostPiece::Opacity, AGhostPiece::TranslucentSortPriority);
}

void AGhostPiece::AttachToMatrix(USceneComponent* const MatrixRoot)
{
	if (RootComponent)
	{
		RootComponent->AttachToComponent(MatrixRoot, FAttachmentTransformRules::KeepRelativeTransform);
	}
}
