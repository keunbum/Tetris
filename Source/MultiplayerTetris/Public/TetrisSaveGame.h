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
	void Initialize();
	void SaveSetting();

	// static methods
	static UTetrisSaveGame* LoadTetrisSaveGame();
	
public:
	// Save Slot Name
	static const FName SlotName;
	static constexpr int32 UserIndex = 0;

	/** Save Data */
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	float MainMenuBGMVolume;
};
