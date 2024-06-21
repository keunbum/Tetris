// Copyright KeunBeom Ryu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TetriminoQueue.generated.h"

class ATetrimino;

/**
 * ATetriminoQueue is an Actor responsible for managing a queue of Tetriminos in the game.
 * It allows Tetriminos to be added to the queue and provides functionality to retrieve
 * and remove the next Tetrimino from the queue when needed. This system is essential
 * for controlling the flow of Tetriminos that the player will place on the game board.
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetriminoQueue : public AActor
{
	GENERATED_BODY()
	
public:	
	ATetriminoQueue();

	/**
	 * Adds a new Tetrimino to the end of the queue.
	 * @param Tetrimino The Tetrimino to add to the queue.
	 */
	void PushTetrimino(ATetrimino* const Tetrimino);

	/**
	 * Removes and returns the next Tetrimino from the queue.
	 * @return The next Tetrimino in the queue, or nullptr if the queue is empty.
	 */
	ATetrimino* PopTetrimino();

private:
	TArray<TObjectPtr<ATetrimino>> TetriminoArray;
};
