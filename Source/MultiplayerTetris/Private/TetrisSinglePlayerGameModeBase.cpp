// Copyright © 2024 Ryu KeunBeom. All Rights Reserved.

#include "TetrisSinglePlayerGameModeBase.h"

#include "TetrisPlayManager.h"

void ATetrisSinglePlayerGameModeBase::LevelUp()
{
	Super::LevelUp();

	const float NewNormalFallSpeed = GetNormalFallSpeed();
	TetrisPlayManager->SetNormalFallSpeed(NewNormalFallSpeed);
}

