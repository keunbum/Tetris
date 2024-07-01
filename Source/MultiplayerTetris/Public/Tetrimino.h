// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TetriminoBase.h"
#include "Tetrimino.generated.h"

/**
 * @class ATetrimino
 * @brief Represents a tetrimino in the game.
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrimino : public ATetriminoBase
{
	GENERATED_BODY()
	
public:
	ATetrimino();
};
