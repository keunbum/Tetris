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

	void SetBoard(ABoard* const InBoard);

private:
	void AttachToMatrix(USceneComponent* const MatrixRoot);
	void DetachFromMatrix();

public:
	static constexpr ETetriminoFacing DefaultFacing = ETetriminoFacing::North;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetriminoBase> GhostPiece;

	UPROPERTY()
	TObjectPtr<ABoard> Board;
};
