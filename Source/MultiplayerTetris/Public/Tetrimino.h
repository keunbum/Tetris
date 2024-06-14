// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "GameFramework/Actor.h"
#include "Math/MathFwd.h"

#include "EnumClassOperators.h"

#include "Tetrimino.generated.h"

class AMino;
class ABoard;
class UMaterialInterface;

UENUM()
enum class ETetriminoShape : int8
{
	None = -1,
	O,
	I,
	T,
	L,
	J,
	S,
	Z,
	Max
};

UENUM()
enum class ETetriminoFacing : uint8
{
	North,
	East,
	South,
	West,
	Max
};

ENUM_CLASS_OPERATORS(ETetriminoFacing)

struct FTetriminoShapeInfo
{
	TMap<ETetriminoFacing, TArray<FIntPoint>> MinoLocalMatrixLocationsByFacing;
	FString MaterialPath;
	FIntPoint InitialMatrixLocation;
	TMap<ETetriminoFacing, TArray<FIntPoint>> SRSRotationPointOffsetsTable; // Super Rotation System Rotation Point Table
};

UCLASS()
class MULTIPLAYERTETRIS_API ATetrimino : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATetrimino();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

public:
	const FTetriminoShapeInfo& GetTetriminoShapeInfo() const;
	const FIntPoint& GetInitialMatrixLocation() const;
	const TArray<FIntPoint>& GetMinoLocalMatrixLocations() const;
	/** @param RotationDirection - +1: 시계 방향, -1: 반시계 방향 */
	const TArray<FIntPoint>& GetSRSRotationPointOffsets(const int32 RotationDirection) const;

	const ETetriminoShape& GetShape() const { return Shape; }
	const ETetriminoFacing& GetFacing() const { return Facing; }
	const FIntPoint& GetMatrixLocation() const { return MatrixLocation; }
	const TArray<TObjectPtr<AMino>>& GetMinoArray() const { return MinoArray; }

	void Initialize(const ETetriminoShape NewTetriminoShape);
	void SetShape(const ETetriminoShape NewShape) { Shape = NewShape; }
	void MoveBy(const FIntPoint& IntVector2D);
	void RotateTo(const int32 RotationDirection);
	void AttachToBoard(ABoard* const Board);

	static ETetriminoShape GetTetriminoShapeRandom();
	static const TArray<FIntPoint>& GetMinoLocalMatrixLocationsByTetriminoShapeAndFacing(const ETetriminoShape Shape, const ETetriminoFacing Facing);

protected:
	void SetFacing(const ETetriminoFacing NewFacing) { Facing = NewFacing; }

	UMaterialInterface* GetMaterial() const;
	void InitializeMinoArray();
	void UpdateMinoLocalMatrixLocations();

	void DebugPrintState() const;

	static const FTetriminoShapeInfo& GetTetriminoShapeInfoByShape(const ETetriminoShape Shape);
	static const FIntPoint& GetInitialMatrixLocationByShape(const ETetriminoShape Shape);

	struct FRotationInfo
	{
		ETetriminoShape Shape;
		ETetriminoFacing Facing;
		int32 Direction;
	};
	static const TArray<FIntPoint>& GetSRSRotationPointOffsetsByRotationInfo(const FRotationInfo& RotationInfo);
	static UMaterialInterface* GetMaterialByTetriminoShapeInfo(const FTetriminoShapeInfo& TetriminoShapeInfo);
	static FString GetTetriminoShapeName(const ETetriminoShape Shape);
	static FString GetFacingName(const ETetriminoFacing Facing);

public:
	static constexpr int32 MinoNum = 4;
	static const FVector2D MoveDirectionLeft;
	static const FVector2D MoveDirectionRight;
	static const FVector2D MoveDirectionDown;

	static const TMap<ETetriminoShape, FTetriminoShapeInfo> TetriminoShapeInfos;

private:
	UPROPERTY(VisibleAnywhere)
	ETetriminoShape Shape;

	UPROPERTY(VisibleAnywhere)
	ETetriminoFacing Facing;

	UPROPERTY(VisibleAnywhere)
	FIntPoint MatrixLocation;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AMino> MinoClass;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AMino>> MinoArray;

	UPROPERTY(EditDefaultsOnly)
	bool bIsGhostPieceOn;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetrimino> GhostPiece;
};
