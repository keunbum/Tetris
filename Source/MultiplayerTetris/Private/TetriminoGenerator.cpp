// Fill out your copyright notice in the Description page of Project Settings.


#include "TetriminoGenerator.h"

#include "Algo/RandomShuffle.h"

#include "TetriminoBase.h"

UTetriminoGenerator::UTetriminoGenerator()
{
	BagCurrentIndex = 0;
	InitializeBag();
}

ATetriminoBase* UTetriminoGenerator::SpawnTetriminoByBagSystem(UClass* const TetriminoClass)
{
	// Check if the bag has been exhausted
	if (BagCurrentIndex >= TetriminoBag.Num())
	{
		BagCurrentIndex = 0;
		ShuffleBag();
	}

	// Get the next Tetrimino shape from the bag
	const ETetriminoShape NextTetriminoShape = TetriminoBag[BagCurrentIndex++];
	return SpawnTetriminoByShape(TetriminoClass, NextTetriminoShape);
}

ATetriminoBase* UTetriminoGenerator::SpawnTetriminoByShape(UClass* const TetriminoClass, const ETetriminoShape TetriminoShape)
{
	// Spawn the Tetrimino actor in the world
	ATetriminoBase* const NewTetrimino = GetWorld()->SpawnActor<ATetriminoBase>(TetriminoClass);
	NewTetrimino->Initialize(TetriminoShape);
	return NewTetrimino;
}

void UTetriminoGenerator::InitializeBag()
{
	const int32 TetriminoShapeNum = static_cast<int32>(ETetriminoShape::Max);
	TetriminoBag.Reserve(TetriminoShapeNum);
	// Add all Tetrimino shapes to the bag
	for (int32 TetriminoShapeIndex = 0; TetriminoShapeIndex < TetriminoShapeNum; ++TetriminoShapeIndex)
	{
		TetriminoBag.Add(static_cast<ETetriminoShape>(TetriminoShapeIndex));
	}
	ShuffleBag();
}

void UTetriminoGenerator::ShuffleBag()
{
	// Shuffle the bag using the Fisher-Yates algorithm
	for (int32 BagIndex = TetriminoBag.Num() - 1; BagIndex > 0; --BagIndex)
	{
		const int32 RandomIndex = FMath::RandRange(0, BagIndex);
		TetriminoBag.Swap(BagIndex, RandomIndex);
	}
}
