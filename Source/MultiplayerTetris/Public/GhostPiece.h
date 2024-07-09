// Copyright © 2024 Ryu KeunBeom. All Rights Reserved.

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
	void AttachToMatrix(USceneComponent* const MatrixRoot);
};
