// Copyright Ryu KeunBeom. All Rights Reserved.

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

public:
	static constexpr float Opacity = 0.5f;
	static constexpr int32 TranslucentSortPriority = 0;
};
