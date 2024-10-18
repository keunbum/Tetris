// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetriminoBase.h"
#include "TetriminoGhostPiece.generated.h"

class ABoard;

/**
 * 
 */
UCLASS()
class TETRIS_API ATetriminoGhostPiece : public ATetriminoBase
{
	GENERATED_BODY()

public:
	virtual const FMinoInfo GetMinoInfo() const override;

	void AttachToMatrix(USceneComponent* const MatrixRoot);

private:
	static constexpr float Opacity = 0.1f;
	static constexpr int32 TranslucentSortPriority = 0;
};
