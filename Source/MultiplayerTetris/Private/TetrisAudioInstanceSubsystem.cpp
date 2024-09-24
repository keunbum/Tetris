// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisAudioInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "TetrisSaveGameOption.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

const FName UTetrisAudioInstanceSubsystem::MainSoundMixName(TEXT("/Game/Audio/MainSoundMix"));
const FName UTetrisAudioInstanceSubsystem::MainSoundClassName(TEXT("/Game/Audio/MainSoundClass"));
const FName UTetrisAudioInstanceSubsystem::BGMSoundClassName(TEXT("/Game/Audio/BGMSoundClass"));

void UTetrisAudioInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	MainSoundMix = LoadObject<USoundMix>(nullptr, *MainSoundMixName.ToString());
	check(MainSoundMix != nullptr);
	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			UGameplayStatics::SetBaseSoundMix(GetWorld(), MainSoundMix);
		});

	MainSoundClass = LoadObject<USoundClass>(nullptr, *MainSoundClassName.ToString());
	check(MainSoundClass != nullptr);

	BGMSoundClass = LoadObject<USoundClass>(nullptr, *BGMSoundClassName.ToString());
	check(BGMSoundClass != nullptr);

	LoadSavedSettings();
}

void UTetrisAudioInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();

	if (SaveCommonOption)
	{
		SaveCommonOption->SaveCommonOptionSettings();
	}
}

float UTetrisAudioInstanceSubsystem::GetMainSoundClassVolume() const
{
	return SaveCommonOption->GetMainSoundClassVolume();
}

float UTetrisAudioInstanceSubsystem::GetBGMSoundClassVolume() const
{
	return SaveCommonOption->GetBGMSoundClassVolume();
}

void UTetrisAudioInstanceSubsystem::SetMainSoundClassVolume(const float NewVolume)
{
	SaveCommonOption->SetMainSoundClassVolume(NewVolume);
	UTetrisAudioInstanceSubsystem::SetSoundClassVolume(MainSoundClass, NewVolume);
}

void UTetrisAudioInstanceSubsystem::SetBGMSoundClassVolume(const float NewVolume)
{
	SaveCommonOption->SetBGMSoundClassVolume(NewVolume);
	UTetrisAudioInstanceSubsystem::SetSoundClassVolume(BGMSoundClass, NewVolume);
}

void UTetrisAudioInstanceSubsystem::LoadSavedSettings()
{
	if (!SaveCommonOption)
	{
		SaveCommonOption = UTetrisSaveGameOption::LoadTetrisSaveCommonOption();
		check(SaveCommonOption != nullptr);
	}

	SetMainSoundClassVolume(GetMainSoundClassVolume());
	SetBGMSoundClassVolume(GetBGMSoundClassVolume());
}

void UTetrisAudioInstanceSubsystem::SetSoundClassVolume(USoundClass* const SoundClass, const float NewVolume)
{
	if (SoundClass)
	{
		AsyncTask(ENamedThreads::GameThread, [this, SoundClass, NewVolume]()
			{
				UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MainSoundMix, SoundClass, NewVolume, 1.0f, 0.0f, true);
			});
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UTetrisAudioInstanceSubsystem::SetSoundClassVolume() - Invalid SoundClass"));
	}
}
