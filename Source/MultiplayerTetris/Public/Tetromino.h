// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

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
	L,
	J,
	T,
	S,
	Z,
};

UCLASS()
class MULTIPLAYERTETRIS_API ATetromino : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATetromino();

	void SetTetrominoType(const ETetrominoType NewTetrominoType);
	void Move(const FVector2D& Direction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

	virtual void Initialize();

private:
	void InitializeMinos();
	void DebugPrintState() const;

	static FString GetTetrominoTypeName(const ETetrominoType TetrominoType);

public:
	static const FVector2D DirectionLeft;
	static const FVector2D DirectionRight;
	static const FVector2D DirectionDown;

	struct FTetrominoInfo
	{
		TArray<FVector2D> MinoPositions;
		FString MaterialPath;
	};
	static const TArray<FTetrominoInfo> Infos;

private:
	UPROPERTY(VisibleAnywhere)
	ETetrominoType TetrominoType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMino> MinoClass;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AMino>> Minos;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetromino> GhostPiece;
};
