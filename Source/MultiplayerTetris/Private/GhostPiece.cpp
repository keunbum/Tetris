// Copyright © 2024 Ryu KeunBeom. All Rights Reserved.

#include "GhostPiece.h"

#include "Board.h"

void AGhostPiece::AttachToBoard(ABoard* const Board)
{
	USceneComponent* const NewParentComponent = Board->GetMatrixRoot();
	check(NewParentComponent != nullptr);
	check(RootComponent != nullptr);
	RootComponent->AttachToComponent(NewParentComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
