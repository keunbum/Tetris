// Copyright KeunBeom Ryu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "GameFramework/Actor.h"
#include "Math/MathFwd.h"

#include "EnumClassOperators.h"

#include "TetriminoBase.generated.h"

struct FMinoInfo;
class UMino;
class ABoard;
class UMino;
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

UENUM()
enum class ETetriminoRotationDirection : int8
{
	CounterClockwise = -1,
	Clockwise = 1
};

struct FTetriminoShapeInfo
{
	TMap<ETetriminoFacing, TArray<FIntPoint>> MinoMatrixLocalLocationsByFacing;
	FString MaterialPath;
	FLinearColor Color;
	FIntPoint InitialMatrixLocation;
	TMap<ETetriminoFacing, TMap<ETetriminoRotationDirection, TArray<FIntPoint>>> SRSRotationPointOffsetsTable; // Super Rotation System Rotation Point Table
};

/**
 * @class ATetriminoBase
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetriminoBase : public AActor
{
	GENERATED_BODY()

public:
	ATetriminoBase();

	const FMinoInfo GetMinoInfo() const;
	const FTetriminoShapeInfo& GetTetriminoShapeInfo() const;
	const FIntPoint& GetInitialMatrixLocation() const;
	const TArray<FIntPoint>& GetMinoMatrixLocalLocations() const;
	const TArray<FIntPoint>& GetSRSRotationPointOffsets(const ETetriminoRotationDirection RotationDirection) const;

	const ETetriminoShape& GetShape() const { return Shape; }
	const ETetriminoFacing& GetFacing() const { return Facing; }
	const FIntPoint& GetMatrixLocation() const { return MatrixLocation; }
	const TArray<UMino*>& GetMinoArray() const { return MinoArray; }

	void SetShape(const ETetriminoShape NewShape) { Shape = NewShape; }
	void SetFacing(const ETetriminoFacing NewFacing) { Facing = NewFacing; }
	void SetRelativeLocationByMatrixLocation(const FIntPoint& NewMatrixLocation);

	void Initialize(const ETetriminoShape NewTetriminoShape);
	void UpdateMinoMatrixLocalLocations();
	void DetachMinos();

	void DebugPrintState() const;

	// static method
	static ETetriminoShape GetTetriminoShapeRandom();
	static const TArray<FIntPoint>& GetMinoMatrixLocalLocationsByTetriminoShapeAndFacing(const ETetriminoShape Shape, const ETetriminoFacing Facing);
	static FIntPoint GetMovementIntPoint2D(const FVector2D& Direction);

protected:
	void SetMatrixLocation(const FIntPoint& NewMatrixLocation) { MatrixLocation = NewMatrixLocation; }

	void InitializeMinoArray();
	void DestroyMinos();

	void AttachToComponentByMatrixLocation(USceneComponent* const NewParentComponent, const FIntPoint& InitialMatrixLocation);

	static const FTetriminoShapeInfo& GetTetriminoShapeInfoByShape(const ETetriminoShape Shape);
	static const FIntPoint& GetInitialMatrixLocationByShape(const ETetriminoShape Shape);

	struct FRotationInfo
	{
		ETetriminoShape Shape;
		ETetriminoFacing Facing;
		ETetriminoRotationDirection Direction;
	};
	static const TArray<FIntPoint>& GetSRSRotationPointOffsetsByRotationInfo(const FRotationInfo& RotationInfo);
	static FString GetTetriminoShapeName(const ETetriminoShape Shape);
	static FString GetFacingName(const ETetriminoFacing Facing);

public:
	static constexpr int32 MinoNum = 4;
	static const FVector2D MoveDirectionLeft;
	static const FVector2D MoveDirectionRight;
	static const FVector2D MoveDirectionDown;

	static const TMap<ETetriminoShape, FTetriminoShapeInfo> TetriminoShapeInfos;

protected:
	UPROPERTY(VisibleAnywhere)
	ETetriminoShape Shape;

	UPROPERTY(VisibleAnywhere)
	ETetriminoFacing Facing;

	UPROPERTY(VisibleAnywhere)
	FIntPoint MatrixLocation;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UMino> MinoClass;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMino>> MinoArray;
};
