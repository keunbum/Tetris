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

	const TArray<FIntPoint>& GetMinoLocalMatrixLocationsByFacing(const ETetriminoFacing Facing) const
	{
		return MinoLocalMatrixLocationsByFacing[Facing];
	}
};

UCLASS()
class MULTIPLAYERTETRIS_API ATetrimino : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATetrimino();

	const TArray<FIntPoint>& GetMinoLocalMatrixLocations() const;
	const FIntPoint& GetInitialMatrixLocation() const;
	const ETetriminoShape GetTetriminoShape() const { return TetriminoShape; }
	const FIntPoint& GetMatrixLocation() const { return MatrixLocation; }
	const TArray<TObjectPtr<AMino>>& GetMinoArray() const { return MinoArray; }

	void Initialize(const ETetriminoShape NewTetriminoShape);
	void SetTetriminoShape(const ETetriminoShape NewTetriminoShape) { TetriminoShape = NewTetriminoShape; }
	void MoveBy(const FIntPoint& IntVector2D);
	void RotateBy(const int32 Value);
	void AttachToBoard(ABoard* const Board);

	static ETetriminoShape GetTetriminoShapeRandom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

protected:
	void SetFacing(const ETetriminoFacing NewFacing) { Facing = NewFacing; }

	void InitializeMinoArray(const FTetriminoInfo& TetriminoInfo);
	void UpdateMinoPositions();

	void DebugPrintState() const;

	static const FTetriminoInfo& GetTetriminoInfoByShape(const ETetriminoShape TetriminoShape);
	static UMaterialInterface* GetMaterialByTetriminoInfo(const FTetriminoInfo& TetriminoInfo);
	static FString GetTetriminoShapeName(const ETetriminoShape TetriminoShape);
	static FString GetFacingName(const ETetriminoFacing Facing);

public:
	static constexpr int32 MinoNum = 4;
	static const FVector2D MoveDirectionLeft;
	static const FVector2D MoveDirectionRight;
	static const FVector2D MoveDirectionDown;

	static const TMap<ETetriminoShape, FTetriminoInfo> TetriminoInfos;

private:
	UPROPERTY(VisibleAnywhere)
	ETetriminoShape TetriminoShape;

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
