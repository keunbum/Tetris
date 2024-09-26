// Copyright Ryu KeunBeom. All Rights Reserved.


#include "TetriminoQueue.h"

#include "Tetrimino.h"

ATetriminoQueue::ATetriminoQueue()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ATetriminoQueue::Initialize(const int32 QueueSize, USceneComponent* const NewParentComponent)
{
	TetriminoArray.Reserve(QueueSize);
	if (RootComponent)
	{
		RootComponent->AttachToComponent(NewParentComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void ATetriminoQueue::Enqueue(ATetrimino* const Tetrimino)
{
	if (Tetrimino)
	{
		TetriminoArray.Add(Tetrimino);
		// Attach the Tetrimino to the queue's root component
		Tetrimino->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

ATetrimino* ATetriminoQueue::Dequeue()
{
	if (TetriminoArray.IsEmpty())
	{
		return nullptr;
	}

	ATetrimino* const NextTetrimino = TetriminoArray[0];
	TetriminoArray.RemoveAt(0);

	// Detach the Tetrimino from the queue's root component
	if (NextTetrimino)
	{
		NextTetrimino->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	}

	return NextTetrimino;
}

void ATetriminoQueue::ReArrangeTetriminoLocations()
{
	for (int32 Index = 0; Index < TetriminoArray.Num(); ++Index)
	{
		if (ATetrimino* const Tetrimino = TetriminoArray[Index])
		{
			const FVector NewLocation = ATetriminoQueue::GetTetriminoLocalLocationByIndex(Index);
			Tetrimino->SetActorRelativeLocation(NewLocation);
		}
	}
}

FVector ATetriminoQueue::GetTetriminoLocalLocationByIndex(const int32 Index)
{
	return FVector(0.0f, TetriminoYOffset * Index, 0.0f);
}
