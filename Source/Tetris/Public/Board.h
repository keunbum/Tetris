// Copyright Ryu KeunBeom. All Rights Reserved.

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
 * @brief Represents the game board in the Tetris.
 */
UCLASS()
class TETRIS_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:
	ABoard();

	/** Initializes */
	void Initialize();

	/** Check Methods */
	// Determine if the Tetrimino directly above a surface (the floor or another block)?
	bool IsDirectlyAboveSurface(const ATetrimino* Tetrimino) const;
	// Determine if the Tetrimino is blocked due to an existing Block in the Matrix.
	bool IsBlocked(const ATetrimino* Tetrimino) const;
	// Determine if the Tetrimino is above the skyline.
	bool IsAboveSkyline(const ATetrimino* Tetrimino) const;
	bool IsMovementPossible(const ATetrimino* Tetrimino, const FIntPoint& MovementIntPoint2D) const;
	bool IsRotationPossible(const ATetrimino* Tetrimino, const ETetriminoRotationDirection RotationDirection, const FIntPoint& RotationPointOffset) const;
	bool IsRowFull(const int32 TargetRow) const;

	/** Get Methods */
	USceneComponent* GetMatrixRoot() const { return MatrixRoot; }
	USceneComponent* GetNextQueueRoot() const { return NextQueueRoot; }
	USceneComponent* GetHoldQueueRoot() const { return HoldQueueRoot; }

	/** Non-const Methods */
	void AddMinos(const ATetrimino* Tetrimino);
	// HitList에 있는 Row를 제거하고, 위에 있는 Row를 아래로 내린다.
	void ClearRows(const TArray<int32>& TargetRows);

	/** Const Methods */
	// Returns the final falling location of the Tetrimino in the matrix.
	FIntPoint GetFinalFallingMatrixLocation(const ATetrimino* Tetrimino) const;

private:
	/** Initializes */
	void InitializeBackground();
	void InitializeMinoMatrix();

	/** Check Methods */
	bool IsMatrixLocationEmpty(const FIntPoint& MatrixLocation) const;
	bool IsMinoLocationsPossible(const TArray<FIntPoint>& MinoLocalMatrixLocations, const FIntPoint& TetriminoMatrixLocation) const;

	/** Get/Set Methods */
	int32 GetMatrixIndexByMatrixLocation(const FIntPoint& MatrixLocation) const;
	UMino* GetMinoByMatrixLocation(const FIntPoint& MatrixLocation) const;
	void SetMinoByMatrixLocation(UMino* const Mino, const FIntPoint& MatrixLocation);

	/** Non-const Methods */
	USceneComponent* CreateAndSetupSceneComponent(const FName& ComponentName, USceneComponent* const Parent);
	void AddMino(UMino* const Mino, const FIntPoint& MinoMatrixLocation);
	void ClearRow(const int32 TargetRow);
	void MoveRow(const int32 TargetRow, const int32 MoveDistance);
	void RemoveMino(UMino* const Mino, const FIntPoint& MinoMatrixLocation);
	void MoveMino(UMino* const Mino, const FIntPoint& OldMatrixLocation, const FIntPoint& NewMatrixLocation);

public:
	static constexpr int32 TotalHeight = 40;
	static constexpr int32 TotalWidth = 10;
	/** [TotalBeginRow, TotalEndRow) */
	static constexpr int32 TotalBeginRow = 0;
	/** [TotalBeginRow, TotalEndRow) */
	static constexpr int32 TotalEndRow = TotalBeginRow + TotalHeight;
	/** [TotalBeginCol, TotalEndCol) */
	static constexpr int32 TotalBeginCol = 0;
	/** [TotalBeginCol, TotalEndCol) */
	static constexpr int32 TotalEndCol = TotalBeginCol + TotalWidth;

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

	static constexpr int32 SkyLine = VisibleBeginRow;

	static constexpr int32 TetriminoDefaultSpawnLocationX = ABoard::SkyLine;
	static constexpr int32 TetriminoDefaultSpawnLocationY = ABoard::VisibleBeginCol + 3;

	static constexpr int32 MatrixVisibleWidth = VisibleWidth * UMino::UnitLength;
	static constexpr int32 MatrixVisibleHeight = VisibleHeight * UMino::UnitLength;

private:
	static const FMinoInfo BackgroundMinoInfo;
	static const FMinoInfo SpecialMinoInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Classes");
	TSubclassOf<UMino> MinoClass;

	UPROPERTY(EditInstanceOnly, Category = "Location")
	FVector MatrixRelativeLocation;

	UPROPERTY(EditInstanceOnly, Category = "Location")
	FVector NextQueueRelativeLocation;

	UPROPERTY(EditInstanceOnly, Category = "Location")
	FVector HoldQueueRelativeLocation;

	UPROPERTY(EditInstanceOnly, Category = "USceneComponent")
	TObjectPtr<USceneComponent> MatrixRoot;

	UPROPERTY(EditInstanceOnly, Category = "USceneComponent")
	TObjectPtr<USceneComponent> BackgroundRoot;

	UPROPERTY(EditInstanceOnly, Category = "USceneComponent")
	TObjectPtr<USceneComponent> NextQueueRoot;

	UPROPERTY(EditInstanceOnly, Category = "USceneComponent")
	TObjectPtr<USceneComponent> HoldQueueRoot;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMino>> MinoMatrix;
};
