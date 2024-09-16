// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TetrisSaveGameOption.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UTetrisSaveGameOption : public USaveGame
{
	GENERATED_BODY()

public:
	void Initialize();
	void SaveSetting();

	// SaveSetting은 옵션창 닫으며 최종적으로 저장할 때 호출
	void SetBGMVolume(const float NewVolume) { BGMVolume = NewVolume; }
	float GetBGMVolume() const { return BGMVolume; }

	// static methods
	static UTetrisSaveGameOption* LoadTetrisSaveGame();
	
public:
	static const FName SlotName;
	static constexpr int32 UserIndex = 0;

private:
	/** Save Data */
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	float BGMVolume;
};
