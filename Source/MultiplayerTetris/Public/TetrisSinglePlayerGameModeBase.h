// Copyright © 2024 Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisGameModeBase.h"
#include "TetrisSinglePlayerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisSinglePlayerGameModeBase : public ATetrisGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void LevelUp() override;
};
