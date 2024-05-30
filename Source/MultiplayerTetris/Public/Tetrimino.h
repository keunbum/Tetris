// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EnumClassOperators.h"

#include "Tetrimino.generated.h"

class AMino;
class ATetrisGameModeBase;
class UMaterialInterface;

UENUM()
enum class ETetriminoType : int8
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
enum class ETetriminoFacingType : uint8
{
	North,
	East,
	South,
	West,
	Max
};

ENUM_CLASS_OPERATORS(ETetriminoFacingType)

struct FTetrominoInfo
{
	TMap<ETetriminoFacingType, TArray<FVector2D>> MinoUnitPositionsByFacing;
	FString MaterialPath;

	const TArray<FVector2D>& GetMinoUnitPosition(const ETetriminoFacingType Facing)
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

	void Initialize(const ETetriminoType NewTetrominoType);
	void SetTetriminoType(const ETetriminoType NewTetriminoType) { TetriminoType = NewTetriminoType; }
	void Move(const FVector2D& Direction2D);
	void RotateTo(const int32 Direction);

	static ETetriminoType GetTetriminoTypeRandom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

protected:
	void SetFacingType(const ETetriminoFacingType NewFacingType) { FacingType = NewFacingType; }

	void InitializeMinos();
	void UpdateMinoPositions();

	void DebugPrintState() const;

	static bool GetTetrominoInfo(FTetrominoInfo& OutTetrominoInfo, const ETetriminoType TetriminoType);
	static UMaterialInterface* GetMaterialByTetrominoInfo(const FTetrominoInfo& TetrominoInfo);
	static FString GetTetrominoTypeName(const ETetriminoType TetriminoType);
	static FString GetFacingTypeName(const ETetriminoFacingType FacingType);
public:
	static constexpr int32 MinoNum = 4;
	static const FVector2D MoveDirectionLeft;
	static const FVector2D MoveDirectionRight;
	static const FVector2D MoveDirectionDown;

	static const TMap<ETetriminoType, FTetrominoInfo> TetrominoInfos;

private:
	UPROPERTY(VisibleAnywhere)
	ETetriminoType TetriminoType;

	UPROPERTY(VisibleAnywhere)
	ETetriminoFacingType FacingType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMino> MinoClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AMino> Minos[MinoNum];

	UPROPERTY(EditDefaultsOnly)
	bool bIsGhostPieceOn;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetrimino> GhostPiece;
};
