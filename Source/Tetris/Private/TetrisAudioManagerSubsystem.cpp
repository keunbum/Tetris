// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisAudioManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Async/Async.h"
#include "Misc/ConfigCacheIni.h"

const FName UTetrisAudioManagerSubsystem::MainSoundMixPath(TEXT("/Game/Audio/MainSoundMix"));
const FName UTetrisAudioManagerSubsystem::MainSoundClassPath(TEXT("/Game/Audio/MainSoundClass"));
const FName UTetrisAudioManagerSubsystem::BgmSoundClassPath(TEXT("/Game/Audio/BgmSoundClass"));

const FString UTetrisAudioManagerSubsystem::AudioConfigSectionName(TEXT("/Script/Tetris.AudioManagerSubsystem"));
const FString UTetrisAudioManagerSubsystem::AudioConfigFileName(GGameUserSettingsIni);

void UTetrisAudioManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	InitializeSoundMixAndClasses();

	LoadSettings();
}

void UTetrisAudioManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UTetrisAudioManagerSubsystem::SetMainVolume(const float NewVolume)
{
	SetSoundClassVolume(MainSoundClass, NewVolume);
}

void UTetrisAudioManagerSubsystem::SetBgmVolume(const float NewVolume)
{
	// Print Previous Volume
	UE_LOG(LogTemp, Log, TEXT("UTetrisAudioManagerSubsystem::SetBgmVolume() - Previous Volume: %f"), GetSoundClassVolume(BgmSoundClass));
	UE_LOG(LogTemp, Log, TEXT("UTetrisAudioManagerSubsystem::SetBgmVolume() - New Volume: %f"), NewVolume);
	SetSoundClassVolume(BgmSoundClass, NewVolume);
}

void UTetrisAudioManagerSubsystem::SaveSettings()
{
	// Use ConfigCacheIni to save settings to ini file
	if (GConfig)
	{
		for (const auto& [SoundClassName, Volume] : SoundClassVolumes)
		{
			UE_LOG(LogTemp, Log, TEXT("UTetrisAudioManagerSubsystem::SaveSettings() - SoundClass: %s, Volume: %f"), *SoundClassName.ToString(), Volume);
			GConfig->SetFloat(*AudioConfigSectionName, *SoundClassName.ToString(), Volume, AudioConfigFileName);
		}
		GConfig->Flush(false, AudioConfigFileName);
	}
}

void UTetrisAudioManagerSubsystem::InitializeSoundMixAndClasses()
{
	// MainSoundMix
	MainSoundMix = LoadObject<USoundMix>(nullptr, *MainSoundMixPath.ToString());
	if (MainSoundMix)
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				UGameplayStatics::SetBaseSoundMix(GetWorld(), MainSoundMix);
			});
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UTetrisAudioManagerSubsystem::Initialize() - Failed to load MainSoundMix"));
	}

	MainSoundClass = LoadSoundClassObject(MainSoundClassPath);
	BgmSoundClass = LoadSoundClassObject(BgmSoundClassPath);
}

void UTetrisAudioManagerSubsystem::LoadSettings()
{
	LoadSoundClassVolumeSetting(GConfig, MainSoundClass);
	LoadSoundClassVolumeSetting(GConfig, BgmSoundClass);
}

USoundClass* UTetrisAudioManagerSubsystem::LoadSoundClassObject(const FName& Path)
{
	if (USoundClass* const SoundClass = LoadObject<USoundClass>(nullptr, *Path.ToString()))
	{
		static constexpr float DefaultVolume = 1.0f;
		UE_LOG(LogTemp, Log, TEXT("UTetrisAudioManagerSubsystem::LoadSoundClassObject() - SoundClass: %s, DefaultVolume: %f"), *SoundClass->GetName(), DefaultVolume);
		SoundClassVolumes.Add(SoundClass->GetFName(), DefaultVolume);
		//SetSoundClassVolume(SoundClass, DefaultVolume);
		return SoundClass;
	}

	checkNoEntry();
	return nullptr;
}

void UTetrisAudioManagerSubsystem::LoadSoundClassVolumeSetting(FConfigCacheIni* const Config, USoundClass* const SoundClass)
{
	if (SoundClass)
	{
		if (float Volume = GetSoundClassVolume(SoundClass);
			Config && Config->GetFloat(*AudioConfigSectionName, *SoundClass->GetName(), Volume, AudioConfigFileName))
		{
			UE_LOG(LogTemp, Log, TEXT("UTetrisAudioManagerSubsystem::LoadSoundClassVolumeSetting() - SoundClass: %s, Volume: %f"), *SoundClass->GetName(), Volume);
			SetSoundClassVolume(SoundClass, Volume);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UTetrisAudioManagerSubsystem::LoadSoundClassVolumeSetting() - Failed to load SoundClass volume, set to default value"));
			Config->SetFloat(*AudioConfigSectionName, *SoundClass->GetName(), Volume, AudioConfigFileName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UTetrisAudioManagerSubsystem::LoadSoundClassVolumeSetting() - Invalid SoundClass"));
	}

}

void UTetrisAudioManagerSubsystem::SetSoundClassVolume(USoundClass* const SoundClass, const float NewVolume)
{
	SoundClassVolumes.Add(SoundClass->GetFName(), NewVolume);
	SetSoundMixClassOverrideInGameThread(MainSoundMix, SoundClass, NewVolume);
}

float UTetrisAudioManagerSubsystem::GetSoundClassVolume(USoundClass* const SoundClass) const
{
	if (const float* Volume = SoundClassVolumes.Find(SoundClass->GetFName()))
	{
		return *Volume;
	}

	checkNoEntry();
	return 0.0f;
}

void UTetrisAudioManagerSubsystem::SetSoundMixClassOverrideInGameThread(USoundMix* const SoundMix, USoundClass* const SoundClass, const float Volume, const float Pitch, const float FadeInTime, const bool bApplyToChildren)
{
	if (SoundClass)
	{
		AsyncTask(ENamedThreads::GameThread, [this, SoundMix, SoundClass, Volume, Pitch, FadeInTime, bApplyToChildren]()
			{
				UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SoundClass, Volume, Pitch, FadeInTime, bApplyToChildren);
			});
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UTetrisAudioManagerSubsystem::SetSoundMixClassOverrideInGameThread() - Invalid SoundClassName"));
	}
}
