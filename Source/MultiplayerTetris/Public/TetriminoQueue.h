// Copyright KeunBeom Ryu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TetriminoQueue.generated.h"

class ATetrimino;

UCLASS()
class MULTIPLAYERTETRIS_API ATetriminoQueue : public AActor
{
	GENERATED_BODY()
	
public:	
	ATetriminoQueue();

	ATetrimino* PopTetrimino();
	void PushTetrimino(ATetrimino* const Tetrimino);

protected:
	virtual void BeginPlay() override;

private:
	TArray<TObjectPtr<ATetrimino>> TetriminoArray;
};
