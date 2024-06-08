// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EnumClassOperators.h"

#include "Tetrimino.generated.h"

class AMino;
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
	TMap<ETetriminoFacing, TArray<FVector2D>> MinoUnitPositionsByFacing;
	FString MaterialPath;

	const TArray<FVector2D>& GetMinoUnitPosition(const ETetriminoFacing Facing)
	{
		return MinoUnitPositionsByFacing[Facing];
	}
};

UCLASS()
class MULTIPLAYERTETRIS_API ATetrimino : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATetrimino();

	void Initialize(const ETetriminoShape NewTetriminoShape);
	void SetTetriminoShape(const ETetriminoShape NewTetriminoShape) { TetriminoShape = NewTetriminoShape; }
	void MoveBy(const FVector2D& Vector2D);
	void RotateBy(const int32 Value);

	static ETetriminoShape GetTetriminoShapeRandom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

protected:
	void SetFacing(const ETetriminoFacing NewFacing) { Facing = NewFacing; }

	void InitializeMinos();
	void UpdateMinoPositions();

	void DebugPrintState() const;

	static bool GetTetriminoInfoByShape(FTetriminoInfo& OutTetriminoInfo, const ETetriminoShape TetriminoShape);
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
	FIntVector2 MatrixPosition;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMino> MinoClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AMino> Minos[MinoNum];

	UPROPERTY(EditDefaultsOnly)
	bool bIsGhostPieceOn;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetrimino> GhostPiece;
};
