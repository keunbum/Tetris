// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisAudioManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Async/Async.h"
#include "Misc/ConfigCacheIni.h"
#include "SoundControlDataAsset.h"

const FName UTetrisAudioManagerSubsystem::SoundControlDataAssetPath(TEXT("/Game/Audio/SoundControlData"));
const FName UTetrisAudioManagerSubsystem::AudioConfigSectionName(TEXT("/Script/Tetris.AudioManagerSubsystem"));
const FName UTetrisAudioManagerSubsystem::AudioConfigFileName(GGameUserSettingsIni);

void UTetrisAudioManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadSoundControlDataAsset();
	LoadSettings();
}

void UTetrisAudioManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UTetrisAudioManagerSubsystem::SaveSettings()
{
	if (!GConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("%s - Failed to get GConfig"), *FString(__FUNCTION__));
		return;
	}

	for (const auto& [SoundClassName, Volume] : SoundClassVolumes)
	{
		GConfig->SetFloat(*AudioConfigSectionName.ToString(), *SoundClassName.ToString(), Volume, AudioConfigFileName.ToString());
	}
	GConfig->Flush(false, AudioConfigFileName.ToString());
}

void UTetrisAudioManagerSubsystem::LoadSoundControlDataAsset()
{
	// 언리얼 에디터에서 미리 작성한 파일을 읽는다.
	const USoundControlDataAsset* SoundControlDataAsset = LoadObject<USoundControlDataAsset>(nullptr, *SoundControlDataAssetPath.ToString());
	if (!SoundControlDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("%s - Failed to load SoundControlDataAsset"), *FString(__FUNCTION__));
		return;
	}

	// Set Main Sound Mix
	MainSoundMix = SoundControlDataAsset->MainSoundMix;
	if (!MainSoundMix)
	{
		UE_LOG(LogTemp, Error, TEXT("%s - Invalid MainSoundMix"), *FString(__FUNCTION__));
		return;
	}
	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			UGameplayStatics::SetBaseSoundMix(GetWorld(), MainSoundMix);
		});

	// Set Sound Classes
	MainSoundClass = SoundControlDataAsset->MainSoundClass;
	BgmSoundClass = SoundControlDataAsset->BgmSoundClass;
	SfxSoundClass = SoundControlDataAsset->SfxSoundClass;
}

void UTetrisAudioManagerSubsystem::LoadSettings()
{
	LoadSoundClassVolumeSettings();
}

void UTetrisAudioManagerSubsystem::LoadSoundClassVolumeSettings()
{
	if (!GConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("%s - Failed to get GConfig"), *FString(__FUNCTION__));
		return;
	}
	
	for (USoundClass* const SoundClass : { MainSoundClass, BgmSoundClass, SfxSoundClass })
	{
		if (!SoundClass)
		{
			UE_LOG(LogTemp, Error, TEXT("%s - Invalid SoundClass"), *FString(__FUNCTION__));
			return;
		}

		float Volume = UTetrisAudioManagerSubsystem::DefaultVolume;
		if (float VolumeSaved;
			GConfig->GetFloat(*AudioConfigSectionName.ToString(), *SoundClass->GetName(), VolumeSaved, AudioConfigFileName.ToString()))
		{
			// 기존에 저장된 값이 있다면 불러온다.
			Volume = VolumeSaved;
		}
		else
		{
			// 없다면 새로 저장한다.
			GConfig->SetFloat(*AudioConfigSectionName.ToString(), *SoundClass->GetName(), Volume, AudioConfigFileName.ToString());
		}
		SetSoundClassVolume(SoundClass, Volume);
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

	return UTetrisAudioManagerSubsystem::DefaultVolume;
}

void UTetrisAudioManagerSubsystem::SetSoundMixClassOverrideInGameThread(USoundMix* const SoundMix, USoundClass* const SoundClass, const float Volume, const float Pitch, const float FadeInTime, const bool bApplyToChildren)
{
	if (!SoundClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s - Invalid SoundClass"), *FString(__FUNCTION__));
		return;
	}

	AsyncTask(ENamedThreads::GameThread, [this, SoundMix, SoundClass, Volume, Pitch, FadeInTime, bApplyToChildren]()
		{
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SoundClass, Volume, Pitch, FadeInTime, bApplyToChildren);
		});
}
