// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EnumClassOperators.h"

#include "Tetromino.generated.h"

class AMino;
class ATetrisGameModeBase;
class UMaterialInterface;

UENUM()
enum class ETetrominoType : int8
{
	None = -1,
	O,
	I,
	T,
	L,
	J,
	S,
	Z,
};

UENUM()
enum class ETetrominoFacingType : uint8
{
	North,
	East,
	South,
	West,
	Max
};

ENUM_CLASS_OPERATORS(ETetrominoFacingType)

struct FTetrominoInfo
{
	TMap<ETetrominoFacingType, TArray<FVector2D>> MinoUnitPositionsByFacing;

	FString MaterialPath;

	const TArray<FVector2D>& GetMinoUnitPosition(const ETetrominoFacingType FacingType)
	{
		return MinoUnitPositionsByFacing[FacingType];
	}
};

UCLASS()
class MULTIPLAYERTETRIS_API ATetromino : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATetromino();

	void SetTetrominoType(const ETetrominoType NewTetrominoType) { TetrominoType = NewTetrominoType; }
	void Move(const FVector2D& Direction);
	void RotateTo(const int32 Direction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

	virtual void Initialize();

protected:
	void SetFacingType(const ETetrominoFacingType NewFacingType) { FacingType = NewFacingType; }

	void InitializeMinos();
	void UpdateMinoPositions();

	static bool GetTetrominoInfo(FTetrominoInfo& OutInfo, const ETetrominoType TetrominoType);
	static UMaterialInterface* GetMaterial(const FTetrominoInfo& Info);
	static FString GetTetrominoTypeName(const ETetrominoType TetrominoType);
	static FString GetFacingTypeName(const ETetrominoFacingType FacingType);
	void DebugPrintState() const;

public:
	static const FVector2D MoveDirectionLeft;
	static const FVector2D MoveDirectionRight;
	static const FVector2D MoveDirectionDown;

	static TArray<FTetrominoInfo> Infos;

private:
	UPROPERTY(VisibleAnywhere)
	ETetrominoType TetrominoType;

	UPROPERTY(VisibleAnywhere)
	ETetrominoFacingType FacingType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMino> MinoClass;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AMino>> Minos;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetromino> GhostPiece;
};
