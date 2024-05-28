// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "TetrisGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API AMultiplayerTetrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMultiplayerTetrisGameModeBase();

protected:
	virtual void BeginPlay() override;

private:
	void Initialize();
};
