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

struct FTetriminoInfo
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
	const FTetriminoInfo& GetTetriminoInfo() const;
	const FIntPoint& GetInitialMatrixLocation() const;
	const TArray<FIntPoint>& GetMinoLocalMatrixLocations() const;
	const TArray<FIntPoint>& GetSRSRotationPointOffsets() const;

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

	static const FTetriminoInfo& GetTetriminoInfoByShape(const ETetriminoShape Shape);
	static const FIntPoint& GetInitialMatrixLocationByShape(const ETetriminoShape Shape);
	static const TArray<FIntPoint>& GetSRSRotationPointOffsetsByShapeAndFacing(const ETetriminoShape Shape, const ETetriminoFacing Facing);
	static UMaterialInterface* GetMaterialByTetriminoInfo(const FTetriminoInfo& TetriminoInfo);
	static FString GetTetriminoShapeName(const ETetriminoShape Shape);
	static FString GetFacingName(const ETetriminoFacing Facing);

public:
	static constexpr int32 MinoNum = 4;
	static const FVector2D MoveDirectionLeft;
	static const FVector2D MoveDirectionRight;
	static const FVector2D MoveDirectionDown;

	static const TMap<ETetriminoShape, FTetriminoInfo> TetriminoInfos;

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
