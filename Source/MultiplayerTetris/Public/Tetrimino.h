// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetriminoBase.h"
#include "Tetrimino.generated.h"

class AGhostPiece;

/**
 * @class ATetrimino
 * @brief Represents a tetrimino in the game.
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrimino : public ATetriminoBase
{
	GENERATED_BODY()
	
public:
	ATetrimino();

	void SetGhostPiece(AGhostPiece* const InGhostPiece);

	void AttachToBoard(ABoard* const Board);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetriminoBase> GhostPiece;
};
