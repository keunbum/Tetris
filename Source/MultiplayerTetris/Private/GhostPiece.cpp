// Copyright Ryu KeunBeom. All Rights Reserved.

#include "GhostPiece.h"

void AGhostPiece::AttachToMatrix(USceneComponent* const MatrixRoot)
{
	check(MatrixRoot != nullptr);
	check(RootComponent != nullptr);
	RootComponent->AttachToComponent(MatrixRoot, FAttachmentTransformRules::KeepRelativeTransform);
}
