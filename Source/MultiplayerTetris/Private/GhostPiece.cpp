// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostPiece.h"

#include "Board.h"

void AGhostPiece::AttachToBoard(ABoard* const Board)
{
	USceneComponent* const NewParentComponent = Board->GetMatrixRoot();
	check(NewParentComponent != nullptr);
	check(RootComponent != nullptr);
	RootComponent->AttachToComponent(NewParentComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
