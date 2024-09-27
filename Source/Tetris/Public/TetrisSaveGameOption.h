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

	void SetSoundClassVolume(const FName& SoundClassName, const float NewVolume);
	float GetSoundClassVolume(const FName& SoundClassName) const;

	void DebugPrint(const FString& Prefix = TEXT("")) const;

	// static methods
	static UTetrisSaveGameOption* LoadTetrisSaveCommonOption();
	
public:
	static const FName CommonOptionSlotName;
	static constexpr int32 UserIndex = 0;

private:
	/** Save Data */
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	TMap<FName, float> SoundClassVolumeMap;
};
