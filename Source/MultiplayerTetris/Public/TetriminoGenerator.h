// Copyright KeunBeom Ryu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Tetrimino.h"

#include "TetriminoGenerator.generated.h"

/**
 * The UTetriminoGenerator class is responsible for generating Tetriminos
 * in a way that adheres to the 7-bag system used in modern Tetris games.
 * This system ensures that players receive all seven unique Tetrimino shapes
 * in a randomized order before any shape is repeated, enhancing the fairness
 * and predictability of the game.
 *
 * This class is designed to be used as a logical component within the game,
 * managing the sequence of Tetriminos that are to be spawned into the game world.
 * It does not interact directly with the game world, and thus inherits from UObject.
 */
UCLASS()
class MULTIPLAYERTETRIS_API UTetriminoGenerator : public UObject
{
	GENERATED_BODY()
	
public:
	UTetriminoGenerator();

	ATetrimino* SpawnTetriminoByBagSystem(UClass* const TetriminoClass);
	ATetrimino* SpawnTetriminoByShape(UClass* const TetriminoClass, const ETetriminoShape TetriminoShape);

private:
	void InitializeBag();
	void ShuffleBag();

private:
	int32 BagCurrentIndex;

	UPROPERTY()
	TArray<ETetriminoShape> TetriminoBag;
};
