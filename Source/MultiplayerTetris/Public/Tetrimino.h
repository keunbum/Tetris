// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetriminoBase.h"
#include "Tetrimino.generated.h"

class ABoard;
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

	void MoveBy(const FIntPoint& IntVector2D);
	void RotateTo(const ETetriminoRotationDirection RotationDirection);

	void AttachToBoard(ABoard* const Board);

private:
	void SetBoard(ABoard* const InBoard) { Board = InBoard; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetriminoBase> GhostPiece;

	UPROPERTY()
	TObjectPtr<ABoard> Board;
};
