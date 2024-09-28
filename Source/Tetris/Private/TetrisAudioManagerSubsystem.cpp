// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisAudioManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "TetrisSaveGameOption.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

const FName UTetrisAudioManagerSubsystem::MainSoundMixPath(TEXT("/Game/Audio/MainSoundMix"));
const TArray<TPair<FName, FName>> UTetrisAudioManagerSubsystem::SoundClassPaths =
{
	{ TEXT("MainSoundClass"), TEXT("/Game/Audio/MainSoundClass") },
	{ TEXT("BgmSoundClass"), TEXT("/Game/Audio/BgmSoundClass") },
};
const FName UTetrisAudioManagerSubsystem::BgmSoundClassName(TEXT("BgmSoundClass"));

void UTetrisAudioManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

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

	// Sound Classes
	for (const auto& [_, SoundClassPath] : SoundClassPaths)
	{
		if (USoundClass* const SoundClass = LoadObject<USoundClass>(nullptr, *SoundClassPath.ToString()))
		{
			SoundClassMap.Add(SoundClass->GetFName(), SoundClass);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UTetrisAudioManagerSubsystem::Initialize() - Failed to load SoundClass: %s"), *SoundClassPath.ToString());
		}
	}

	LoadSavedSettings();
}

void UTetrisAudioManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();

	if (SaveCommonOption)
	{
		SaveCommonOption->SaveCommonOptionSettings();
	}
}

void UTetrisAudioManagerSubsystem::LoadSavedSettings()
{
	if (!SaveCommonOption)
	{
		SaveCommonOption = UTetrisSaveGameOption::LoadTetrisSaveCommonOption();
	}
}

void UTetrisAudioManagerSubsystem::SetSoundClassVolume(USoundClass* const SoundClass, const float NewVolume)
{
	if (SaveCommonOption)
	{
		SaveCommonOption->SetSoundClassVolume(SoundClass->GetFName(), NewVolume);
	}

	SetSoundMixClassOverrideInGameThread(MainSoundMix, SoundClass, NewVolume);
}

float UTetrisAudioManagerSubsystem::GetSoundClassVolume(const USoundClass* SoundClass) const
{
	return SaveCommonOption ? SaveCommonOption->GetSoundClassVolume(SoundClass->GetFName()) : 0.f;
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
		UE_LOG(LogTemp, Error, TEXT("UTetrisAudioManagerSubsystem::SetSoundMixClassOverrideInGameThread() - Invalid SoundClass"));
	}
}
