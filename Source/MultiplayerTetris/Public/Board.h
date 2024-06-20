// Copyright KeunBeom Ryu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Mino.h"
#include "Tetrimino.h"

#include "Board.generated.h"

struct FMinoInfo;
class UMino;

/**
 * @class ABoard
 * @brief Represents the game board in the Tetris game.
 */
UCLASS()
class MULTIPLAYERTETRIS_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:
	ABoard();
	virtual void Tick(const float DeltaTime) override;

	bool IsMovementPossible(const ATetrimino* Tetrimino, const FIntPoint& MovementIntPoint2D) const;
	bool IsRotationPossible(const ATetrimino* Tetrimino, const ETetriminoRotationDirection RotationDirection, const FIntPoint& RotationPointOffset) const;

protected:
	virtual void BeginPlay() override;

private:
	void Initialize();
	void InitializeBackground();
	void InitializeMinoMatrix();
	int32 GetMatrixIndexByMatrixLocation(const FIntPoint& MatrixLocation) const;
	UMino* GetMinoByMatrixLocation(const FIntPoint& MatrixLocation) const;
	bool IsMatrixLocationEmpty(const FIntPoint& MatrixLocation) const;
	bool IsMinoLocationsPossible(const FIntPoint& TetriminoMatrixLocation, const TArray<FIntPoint>& MinoLocalMatrixLocations) const;

public:
	static constexpr int32 TotalHeight = 40;
	static constexpr int32 TotalWidth = 10;
	/** [TotalBeginRow, TotalHeight) */
	static constexpr int32 TotalBeginRow = 0;
	static constexpr int32 TotalEndRow = TotalBeginRow + TotalHeight;

	static constexpr int32 VisibleHeight = 20;
	static constexpr int32 VisibleWidth = 10;
	/** [VisibleBeginRow, VisibleEndRow) */
	static constexpr int32 VisibleBeginRow = TotalHeight - VisibleHeight;
	/** [VisibleBeginRow, VisibleEndRow) */
	static constexpr int32 VisibleEndRow = VisibleBeginRow + VisibleHeight;
	/** [VisibleBeginCol, VisibleEndCol) */
	static constexpr int32 VisibleBeginCol = 0;
	/** [VisibleBeginCol, VisibleEndCol) */
	static constexpr int32 VisibleEndCol = VisibleBeginCol + VisibleWidth;

	static constexpr int32 TetriminoDefaultSpawnLocationX = ABoard::VisibleBeginRow - 1;
	static constexpr int32 TetriminoDefaultSpawnLocationY = ABoard::VisibleBeginCol + 3;

private:
	static const FMinoInfo BackgroundMinoInfo;
	static const FMinoInfo SpecialMinoInfo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMino> MinoClass;

	UPROPERTY()
	TObjectPtr<USceneComponent> BackgroundRoot;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMino>> MinoMatrix;
};
