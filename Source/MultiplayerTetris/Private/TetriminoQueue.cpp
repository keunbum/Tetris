// Copyright KeunBeom Ryu. All Rights Reserved.


#include "TetriminoQueue.h"

#include "Tetrimino.h"

ATetriminoQueue::ATetriminoQueue()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	check(RootComponent != nullptr);
}

void ATetriminoQueue::Initialize(const int32 QueueSize, USceneComponent* const NewParentComponent)
{
	TetriminoArray.Reserve(QueueSize);
	check(NewParentComponent != nullptr);
	RootComponent->AttachToComponent(NewParentComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ATetriminoQueue::Enqueue(ATetrimino* const Tetrimino)
{
	check(Tetrimino != nullptr);
	TetriminoArray.Add(Tetrimino);
	// Attach the Tetrimino to the queue's root component
	Tetrimino->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
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

void ATetriminoQueue::ReArrangeQueue()
{
	for (int32 Index = 0; Index < TetriminoArray.Num(); ++Index)
	{
		ATetrimino* const Tetrimino = TetriminoArray[Index];
		const FVector NewLocation = ATetriminoQueue::GetTetriminoLocalLocationByIndex(Index);
		Tetrimino->SetActorRelativeLocation(NewLocation);
	}
}

FVector ATetriminoQueue::GetTetriminoLocalLocationByIndex(const int32 Index)
{
	return FVector(0.0f, TetriminoYOffset * Index, 0.0f);
}
