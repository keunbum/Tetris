// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Mino.h"
#include "Tetrimino.h"

#include "Board.generated.h"

struct FMinoInfo;
class UMino;
class UCameraComponent;

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

	/** Static Methods */
	static int32 GetMatrixIndexByMatrixLocation(const FIntPoint& MatrixLocation);

private:
	/** Create */
	void CreateBoardComponents();
	void CreateMatrixWalls();
	void CreateBackgroundMinos();

	/** Initializes */
	void InitializeMinoMatrix();

	/** Check Methods */
	bool IsMatrixLocationEmpty(const FIntPoint& MatrixLocation) const;
	bool IsMinoLocationsPossible(const TArray<FIntPoint>& MinoLocalMatrixLocations, const FIntPoint& TetriminoMatrixLocation) const;

	/** Get/Set Methods */
	UMino* GetMinoByMatrixLocation(const FIntPoint& MatrixLocation) const;
	void SetMinoByMatrixLocation(UMino* const Mino, const FIntPoint& MatrixLocation);

	/** Non-const Methods */
	template<typename TReturnType>
	TReturnType* CreateAndSetupComponent(const FName& ComponentName, USceneComponent* const Parent, const FVector& RelativeLocation);
	USceneComponent* CreateAndSetupSceneComponent(const FName& ComponentName, USceneComponent* const Parent, const FVector& RelativeLocation) { return CreateAndSetupComponent<USceneComponent>(ComponentName, Parent, RelativeLocation); }
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

	static const FName WallMeshPath;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Classes");
	TSubclassOf<UMino> MinoClass;

	UPROPERTY(VisibleAnywhere, Category = "Location")
	FVector MatrixRelativeLocation;

	UPROPERTY(VisibleAnywhere, Category = "Location")
	FVector NextQueueRelativeLocation;

	UPROPERTY(VisibleAnywhere, Category = "Location")
	FVector HoldQueueRelativeLocation;

	UPROPERTY(VisibleAnywhere, Category = "Location")
	FVector WallRelativeLocation;

	UPROPERTY(VisibleAnywhere, Category = "Location")
	FVector CameraRelativeLocation;

	UPROPERTY(VisibleAnywhere, Category = "USceneComponent")
	TObjectPtr<USceneComponent> MatrixRoot;
	
	UPROPERTY(VisibleAnywhere, Category = "USceneComponent")
	TObjectPtr<USceneComponent> NextQueueRoot;

	UPROPERTY(VisibleAnywhere, Category = "USceneComponent")
	TObjectPtr<USceneComponent> HoldQueueRoot;

	UPROPERTY(VisibleAnywhere, Category = "USceneComponent")
	TObjectPtr<USceneComponent> WallRoot;

	UPROPERTY(VisibleAnywhere, Category = "USceneComponent")
	TObjectPtr<USceneComponent> BackgroundRoot;

	UPROPERTY(VisibleAnywhere, Category = "UCameraComponent")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMino>> MinoMatrix;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> Walls;
};

template<typename TReturnType>
inline TReturnType* ABoard::CreateAndSetupComponent(const FName& ComponentName, USceneComponent* const Parent, const FVector& RelativeLocation)
{
	if (TReturnType* const Component = CreateDefaultSubobject<TReturnType>(ComponentName))
	{
		Component->SetupAttachment(Parent);
		Component->SetRelativeLocation(RelativeLocation);
		return Component;
	}
	checkNoEntry();
	return nullptr;
}
