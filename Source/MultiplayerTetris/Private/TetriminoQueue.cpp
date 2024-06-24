// Copyright KeunBeom Ryu. All Rights Reserved.


#include "TetriminoQueue.h"

ATetriminoQueue::ATetriminoQueue()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATetriminoQueue::Enqueue(ATetrimino* const Tetrimino)
{
	TetriminoArray.Add(Tetrimino);
}

ATetrimino* ATetriminoQueue::Dequeue()
{
	if (TetriminoArray.IsEmpty())
	{
		return nullptr;
	}

	ATetrimino* const NextTetrimino = TetriminoArray[0];
	TetriminoArray.RemoveAt(0);
	return NextTetrimino;
}
