/**
 * @file Board.h
 * This file contains the declaration of the ABoard class, which represents the game board in the Tetris game.
 */

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
	/**
  * @brief Sets default values for this actor's properties.
  */
	ABoard();

protected:
	/**
  * @brief Called when the game starts or when spawned.
  */
	virtual void BeginPlay() override;

public:
	/**
  * @brief Called every frame.
  * @param DeltaTime The time since the last frame.
  */
	virtual void Tick(const float DeltaTime) override;

public:
	/**
  * @brief Checks if the movement of a tetrimino is possible.
  * @param Tetrimino The tetrimino to check.
  * @param MovementIntPoint2D The movement direction as a 2D integer point.
  * @return True if the movement is possible, false otherwise.
  */
	bool IsMovementPossible(const ATetrimino* Tetrimino, const FIntPoint& MovementIntPoint2D) const;

	/**
  * @brief Checks if the rotation of a tetrimino is possible.
  * @param Tetrimino The tetrimino to check.
  * @param RotationDirection The rotation direction.
  * @param RotationPointOffset The rotation point offset as a 2D integer point.
  * @return True if the rotation is possible, false otherwise.
  */
	bool IsRotationPossible(const ATetrimino* Tetrimino, const ETetriminoRotationDirection RotationDirection, const FIntPoint& RotationPointOffset) const;

private:
	/**
  * @brief Initializes the game board.
  */
	void Initialize();

	/**
  * @brief Initializes the background of the game board.
  */
	void InitializeBackground();

	/**
  * @brief Initializes the mino matrix of the game board.
  */
	void InitializeMinoMatrix();

	/**
  * @brief Gets the mino at the specified matrix location.
  * @param MatrixLocation The matrix location as a 2D integer point.
  * @return The mino at the specified matrix location, or nullptr if no mino exists at that location.
  */
	UMino* GetMinoByMatrixLocation(const FIntPoint& MatrixLocation) const;

	/**
  * @brief Checks if the specified matrix location is empty.
  * @param MatrixLocation The matrix location as a 2D integer point.
  * @return True if the matrix location is empty, false otherwise.
  */
	bool IsMatrixLocationEmpty(const FIntPoint& MatrixLocation) const;

	/**
  * @brief Checks if the specified mino locations are possible.
  * @param TetriminoMatrixLocation The matrix location of the tetrimino as a 2D integer point.
  * @param MinoLocalMatrixLocations The local matrix locations of the minos as an array of 2D integer points.
  * @return True if the mino locations are possible, false otherwise.
  */
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
