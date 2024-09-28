// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisAudioSettings.h"
#include "Kismet/GameplayStatics.h"
#include "TetrisAudioManagerSubsystem.h"

const FName UTetrisAudioSettings::CommonOptionSlotName(TEXT("CommonOptionSaveSlot"));

void UTetrisAudioSettings::Initialize()
{
	// Initialize SoundClassVolumeMap
	for (const auto& [SoundClassName, _] : UTetrisAudioManagerSubsystem::SoundClassPaths)
	{
		SoundClassVolumeMap.Add(SoundClassName, 1.0f);
	}
}

void UTetrisAudioSettings::SaveCommonOptionSettings()
{
	UGameplayStatics::SaveGameToSlot(this, UTetrisAudioSettings::CommonOptionSlotName.ToString(), UTetrisAudioSettings::UserIndex);
}

void UTetrisAudioSettings::SetSoundClassVolume(const FName& SoundClassName, const float NewVolume)
{
	// Classes that are not registered in advance are not saved.
	if (float* const Value = SoundClassVolumeMap.Find(SoundClassName))
	{
		*Value = NewVolume;
	}
}

float UTetrisAudioSettings::GetSoundClassVolume(const FName& SoundClassName) const
{
	if (const float* const Value = SoundClassVolumeMap.Find(SoundClassName))
	{
		return *Value;
	}

	return 0.0f;
}

void UTetrisAudioSettings::DebugPrint(const FString& Prefix) const
{
	for (const auto& [SoundClassName, Volume] : SoundClassVolumeMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: %s: %f"), *Prefix, *SoundClassName.ToString(), Volume);
	}
}

UTetrisAudioSettings* UTetrisAudioSettings::LoadTetrisSaveCommonOption()
{
	if (UGameplayStatics::DoesSaveGameExist(UTetrisAudioSettings::CommonOptionSlotName.ToString(), UTetrisAudioSettings::UserIndex))
	{
		// Load Save Game Instance
		UTetrisAudioSettings* const ExistingSaveGameOption = Cast<UTetrisAudioSettings>(UGameplayStatics::LoadGameFromSlot(UTetrisAudioSettings::CommonOptionSlotName.ToString(), UTetrisAudioSettings::UserIndex));
		return ExistingSaveGameOption;
	}

	// Create Save Game Instance
	if (UTetrisAudioSettings* const NewSaveGameOption = Cast<UTetrisAudioSettings>(UGameplayStatics::CreateSaveGameObject(UTetrisAudioSettings::StaticClass())))
	{
		NewSaveGameOption->Initialize();
		NewSaveGameOption->SaveCommonOptionSettings();
		return NewSaveGameOption;
	}

	return nullptr;
}
