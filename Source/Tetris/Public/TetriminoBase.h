// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "GameFramework/Actor.h"
#include "Math/MathFwd.h"

#include "EnumClassOperators.h"
#include "Mino.h"

#include "TetriminoBase.generated.h"

class ABoard;

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
enum class ETetriminoFacing : int8
{
	None = -1,
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
	TMap<ETetriminoFacing, TArray<FIntPoint>> MinoTetriminoLocalLocationsByFacing;
	FLinearColor BaseColor;
	FIntPoint InitialMatrixLocation;
	TMap<ETetriminoFacing, TMap<ETetriminoRotationDirection, TArray<FIntPoint>>> SRSRotationPointOffsetsTable; // Super Rotation System Rotation Point Table
};

/**
 * @class ATetriminoBase
 */
UCLASS(Abstract)
class TETRIS_API ATetriminoBase : public AActor
{
	GENERATED_BODY()

public:
	ATetriminoBase();

	virtual const FMinoInfo GetMinoInfo() const PURE_VIRTUAL(ATetriminoBase::GetMinoInfo, return FMinoInfo(););

	const FTetriminoShapeInfo& GetTetriminoShapeInfo() const;
	const FIntPoint& GetInitialMatrixLocation() const;
	const TArray<FIntPoint>& GetMinoTetriminoLocalLocations() const;
	const TArray<FIntPoint>& GetSRSRotationPointOffsets(const ETetriminoRotationDirection RotationDirection) const;

	const ETetriminoShape& GetShape() const { return Shape; }
	const ETetriminoFacing& GetFacing() const { return Facing; }
	const FIntPoint& GetMatrixLocation() const { return MatrixLocation; }
	const TArray<UMino*>& GetMinoArray() const { return MinoArray; }

	void SetShape(const ETetriminoShape NewShape) { Shape = NewShape; }
	void SetFacing(const ETetriminoFacing NewFacing) { Facing = NewFacing; }
	void SetRelativeLocationByMatrixLocation(const FIntPoint& NewMatrixLocation);

	struct FInitializeParams
	{
		ETetriminoShape Shape;
		ETetriminoFacing Facing;

		FInitializeParams() = delete;

		FInitializeParams(const ETetriminoShape InShape, const ETetriminoFacing InFacing)
			: Shape(InShape)
			, Facing(InFacing)
		{
		}
	};
	void Initialize(const FInitializeParams& Params);
	void UpdateMinoTetriminoLocalLocations();
	void DetachMinos();
	void RotateByFacing(const ETetriminoFacing NewFacing);

	void DebugPrintState() const;

	// static method
	static ETetriminoShape GetTetriminoShapeRandom();
	static const TArray<FIntPoint>& GetMinoTetriminoLocalLocationsByTetriminoShapeAndFacing(const ETetriminoShape Shape, const ETetriminoFacing Facing);
	static FIntPoint GetMatrixMovementIntPointByDirection(const FVector2D& Direction);

protected:
	void SetMatrixLocation(const FIntPoint& NewMatrixLocation) { MatrixLocation = NewMatrixLocation; }

	void InitializeMinoArray();
	void DestroyMinos();
	void AddRelativeLocationByMatrixLocationOffset(const FIntPoint& MatrixLocationOffset);

	void AttachToComponentByMatrixLocation(USceneComponent* const NewParentComponent, const FIntPoint& InitialMatrixLocation);

	// static method
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
