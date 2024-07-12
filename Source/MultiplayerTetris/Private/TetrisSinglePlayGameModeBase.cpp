// Copyright © 2024 Ryu KeunBeom. All Rights Reserved.

#include "TetrisSinglePlayGameModeBase.h"

#include "TetrisPlayManager.h"

void ATetrisSinglePlayGameModeBase::LevelUp()
{
	Super::LevelUp();

	const float NewNormalFallSpeed = GetNormalFallSpeed();
	TetrisPlayManager->SetNormalFallSpeed(NewNormalFallSpeed);
}

