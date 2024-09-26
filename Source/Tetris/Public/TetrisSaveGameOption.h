// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TetrisSaveGameOption.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UTetrisSaveGameOption : public USaveGame
{
	GENERATED_BODY()

public:
	void Initialize();

	void SaveCommonOptionSettings();

	void SetMainSoundClassVolume(const float NewVolume) { MainSoundClassVolume = NewVolume; SaveCommonOptionSettings(); }
	float GetMainSoundClassVolume() const { return MainSoundClassVolume; }

	void SetBGMSoundClassVolume(const float NewVolume) { BGMSoundClassVolume = NewVolume; SaveCommonOptionSettings(); }
	float GetBGMSoundClassVolume() const { return BGMSoundClassVolume; }

	void DebugPrint(const FString& Prefix = TEXT("")) const;

	// static methods
	static UTetrisSaveGameOption* LoadTetrisSaveCommonOption();
	
public:
	static const FName CommonOptionSlotName;
	static constexpr int32 UserIndex = 0;

private:
	/** Save Data */
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	float MainSoundClassVolume = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
	float BGMSoundClassVolume = 1.0f;
};
