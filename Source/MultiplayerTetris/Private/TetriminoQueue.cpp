// Copyright KeunBeom Ryu. All Rights Reserved.


#include "TetriminoQueue.h"

ATetriminoQueue::ATetriminoQueue()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	check(RootComponent != nullptr);
}

void ATetriminoQueue::Initialize(const int32 QueueSize, USceneComponent* const ParentComponent)
{
	TetriminoArray.Reserve(QueueSize);
	check(ParentComponent != nullptr);
	RootComponent->AttachToComponent(ParentComponent, FAttachmentTransformRules::KeepRelativeTransform);
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
