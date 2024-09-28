// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TetrisAudioManagerSubsystem.generated.h"

class USoundMix;
class USoundClass;
class UTetrisSaveGameOption;

/**
 * 
 */
UCLASS()
class TETRIS_API UTetrisAudioManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void SetSoundClassVolumeByName(const FName& SoundClassName, const float NewVolume) { SetSoundClassVolume(SoundClassMap.FindRef(SoundClassName), NewVolume); }
	float GetSoundClassVolumeByName(const FName& SoundClassName) const { return GetSoundClassVolume(SoundClassMap.FindRef(SoundClassName)); }

private:
	void LoadSavedSettings();

	void SetSoundClassVolume(USoundClass* const SoundClass, const float NewVolume);
	float GetSoundClassVolume(const USoundClass* SoundClass) const;

	void SetSoundMixClassOverrideByName(const FName& SoundClassName, const float Volume = 1.0f, const float Pitch = 1.0f, const float FadeInTime = 0.0f, const bool bApplyToChildren = true)
	{
		SetSoundMixClassOverrideInGameThread(MainSoundMix, SoundClassMap.FindRef(SoundClassName), Volume, Pitch, FadeInTime, bApplyToChildren);
	}
	void SetSoundMixClassOverrideInGameThread(USoundMix* const SoundMix, USoundClass* const SoundClass, const float Volume = 1.0f, const float Pitch = 1.0f, const float FadeInTime = 0.0f, const bool bApplyToChildren = true);

public:
	static const FName MainSoundMixPath;
	static const TArray<TPair<FName, FName>> SoundClassPaths;
	static const FName BgmSoundClassName;

private:
	/** Sound Mixes */
	UPROPERTY()
	TObjectPtr<USoundMix> MainSoundMix;

	/** Sound Classes */
	UPROPERTY()
	TMap<FName, TObjectPtr<USoundClass>> SoundClassMap;

	/** Save */
	UPROPERTY()
	TObjectPtr<UTetrisSaveGameOption> SaveCommonOption;
};
