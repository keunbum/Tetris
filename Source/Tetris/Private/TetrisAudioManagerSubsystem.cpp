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
	if (GConfig)
	{
		for (const auto& [SoundClassName, Volume] : SoundClassVolumes)
		{
			GConfig->SetFloat(*AudioConfigSectionName.ToString(), *SoundClassName.ToString(), Volume, AudioConfigFileName.ToString());
		}
		GConfig->Flush(false, AudioConfigFileName.ToString());
	}
}

void UTetrisAudioManagerSubsystem::LoadSoundControlDataAsset()
{
	// 언리얼 에디터에서 미리 작성한 파일을 읽는다.
	if (const USoundControlDataAsset* SoundControlDataAsset = LoadObject<USoundControlDataAsset>(nullptr, *SoundControlDataAssetPath.ToString()))
	{
		// Set Main Sound Mix
		MainSoundMix = SoundControlDataAsset->MainSoundMix;
		if (!MainSoundMix)
		{
			UE_LOG(LogTemp, Error, TEXT("%s - Failed to load MainSoundMix"), *FString(__FUNCTION__));
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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s - Failed to load SoundControlDataAsset"), *FString(__FUNCTION__));
	}
}

void UTetrisAudioManagerSubsystem::LoadSettings()
{
	LoadSoundClassVolumeSettings();
}

void UTetrisAudioManagerSubsystem::LoadSoundClassVolumeSettings()
{
	if (GConfig)
	{
		for (USoundClass* const SoundClass : { MainSoundClass, BgmSoundClass, SfxSoundClass })
		{
			if (SoundClass)
			{
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
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s - Invalid SoundClass"), *FString(__FUNCTION__));
			}
		}
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
		UE_LOG(LogTemp, Warning, TEXT("%s - %s: %f"), *FString(__FUNCTION__), *SoundClass->GetName(), *Volume);
		return *Volume;
	}

	return UTetrisAudioManagerSubsystem::DefaultVolume;
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
		UE_LOG(LogTemp, Error, TEXT("%s - Invalid SoundClassName"), *FString(__FUNCTION__));
	}
}
