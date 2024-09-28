// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TetrisAudioManagerSubsystem.generated.h"

class USoundMix;
class USoundClass;

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

	void SetMainVolume(const float NewVolume);
	float GetMainVolume() const { return GetSoundClassVolume(MainSoundClass); }

	void SetBgmVolume(const float NewVolume);
	float GetBgmVolume() const { return GetSoundClassVolume(BgmSoundClass); }

	void SaveSettings();

private:
	void InitializeSoundMixAndClasses();
	void LoadSettings();
	USoundClass* LoadSoundClassObject(const FName& Path);
	void LoadSoundClassVolumeSettings();

	void SetSoundClassVolume(USoundClass* const SoundClass, const float NewVolume);
	float GetSoundClassVolume(USoundClass* const SoundClass) const;

	void SetSoundMixClassOverrideInGameThread(USoundMix* const SoundMix, USoundClass* const SoundClass, const float Volume = 1.0f, const float Pitch = 1.0f, const float FadeInTime = 0.0f, const bool bApplyToChildren = true);

public:
	static const FName MainSoundMixPath;
	static const FName MainSoundClassPath;
	static const FName BgmSoundClassPath;

	static const FString AudioConfigSectionName;
	static const FString AudioConfigFileName;

	static constexpr float DefaultVolume = 0.75f;

private:
	/** Sound Mixes */
	UPROPERTY()
	TObjectPtr<USoundMix> MainSoundMix;

	/** Sound Classes */
	UPROPERTY()
	TObjectPtr<USoundClass> MainSoundClass;

	UPROPERTY()
	TObjectPtr<USoundClass> BgmSoundClass;

	UPROPERTY()
	TMap<FName, float> SoundClassVolumes; // SoundClassName, Volume
};
