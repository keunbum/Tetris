// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Tetrimino.h"

#include "Board.generated.h"

class AMino;

UCLASS()
class MULTIPLAYERTETRIS_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoard();

	bool IsMovementPossible(const ATetrimino* Tetrimino, const FIntPoint& MovementIntPoint2D) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

private:
	void Initialize();
	void InitializeBackground();
	void InitializeMinoMatrix();

	bool IsMovementWithinRange(const ATetrimino* Tetrimino, const FIntPoint& MovementIntPoint2D) const;

	static UMaterialInterface* GetMinoMaterialByPath(const FString& Path);

public:
	static constexpr int32 TotalHeight = 40;
	static constexpr int32 TotalWidth = 10;

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

	static constexpr int32 TetriminoDefaultSpawnLocationX = ABoard::VisibleBeginRow - 1;
	static constexpr int32 TetriminoDefaultSpawnLocationY = ABoard::VisibleBeginCol + 3;

private:
	static const FString BackgroundMinoMaterialPath;
	static const FString SpecialMinoMaterialPath;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMino> MinoClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<AMino>> Background;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AMino>> MinoMatrix;
};
