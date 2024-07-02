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
	void SetIsGhostPieceOn(const bool bInIsGhostPieceOn) { bIsGhostPieceOn = bInIsGhostPieceOn; }

private:
	UPROPERTY(VisibleAnywhere)
	bool bIsGhostPieceOn;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetriminoBase> GhostPiece;
};
