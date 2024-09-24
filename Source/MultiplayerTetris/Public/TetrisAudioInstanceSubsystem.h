// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TetrisAudioInstanceSubsystem.generated.h"

class USoundMix;
class USoundClass;
class UTetrisSaveGameOption;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UTetrisAudioInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void SetMainSoundClassVolume(const float NewVolume);
	float GetMainSoundClassVolume() const;
	void SetBGMSoundClassVolume(const float NewVolume);
	float GetBGMSoundClassVolume() const;

private:
	void LoadSavedSettings();

	void SetSoundClassVolume(USoundClass* const SoundClass, const float NewVolume);

public:
	static const FName MainSoundMixName;
	static const FName MainSoundClassName;
	static const FName BGMSoundClassName;

private:
	/** Sound Mixes */
	UPROPERTY()
	TObjectPtr<USoundMix> MainSoundMix;

	/** Sound Classes */
	UPROPERTY()
	TObjectPtr<USoundClass> MainSoundClass;

	UPROPERTY()
	TObjectPtr<USoundClass> BGMSoundClass;

	/** Save */
	UPROPERTY()
	TObjectPtr<UTetrisSaveGameOption> SaveCommonOption;
};
