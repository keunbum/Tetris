// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TetrisSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UTetrisSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	// Save Slot Name
	static const FName CommonOptionSaveSlotName;
	static constexpr int32 UserIndex = 0;

	// bIsMainMenuBGMOn
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	bool bIsMainMenuBGMOn;
};
