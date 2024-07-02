// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TetriminoBase.h"
#include "GhostPiece.generated.h"

class ABoard;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API AGhostPiece : public ATetriminoBase
{
	GENERATED_BODY()

public:
	void AttachToBoard(ABoard* const Board);
};
