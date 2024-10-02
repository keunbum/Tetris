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
class TETRIS_API ATetrimino : public ATetriminoBase
{
	GENERATED_BODY()
	
public:
	ATetrimino();

	virtual const FMinoInfo GetMinoInfo() const override;

	void SetGhostPiece(AGhostPiece* const InGhostPiece);
	void SetBoard(ABoard* const InBoard);
	int32 GetLowestRow() const;
	void DetachFromBoard();

	void MoveBy(const FIntPoint& IntVector2D);
	void RotateTo(const ETetriminoRotationDirection RotationDirection);
	void RotateToWithPointOffset(const ETetriminoRotationDirection RotationDirection, const FIntPoint& PointOffset);

private:
	void AttachToMatrix(USceneComponent* const MatrixRoot);
	void DetachFromMatrix();

public:
	static constexpr ETetriminoFacing DefaultFacing = ETetriminoFacing::North;

private:
	static constexpr float Opacity = 1.0f;
	static constexpr int32 TranslucentSortPriority = 1;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AGhostPiece> GhostPiece;

	UPROPERTY()
	TObjectPtr<ABoard> Board;
};
