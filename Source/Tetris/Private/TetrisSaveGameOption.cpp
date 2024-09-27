// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisSaveGameOption.h"
#include "Kismet/GameplayStatics.h"
#include "TetrisAudioInstanceSubsystem.h"

const FName UTetrisSaveGameOption::CommonOptionSlotName(TEXT("CommonOptionSaveSlot"));

void UTetrisSaveGameOption::Initialize()
{
	// Initialize SoundClassVolumeMap
	for (const auto& [SoundClassName, _] : UTetrisAudioInstanceSubsystem::SoundClassPaths)
	{
		SoundClassVolumeMap.Add(SoundClassName, 1.0f);
	}
}

void UTetrisSaveGameOption::SaveCommonOptionSettings()
{
	UGameplayStatics::SaveGameToSlot(this, UTetrisSaveGameOption::CommonOptionSlotName.ToString(), UTetrisSaveGameOption::UserIndex);
}

void UTetrisSaveGameOption::SetSoundClassVolume(const FName& SoundClassName, const float NewVolume)
{
	// Classes that are not registered in advance are not saved.
	if (float* const Value = SoundClassVolumeMap.Find(SoundClassName))
	{
		*Value = NewVolume;
	}
}

float UTetrisSaveGameOption::GetSoundClassVolume(const FName& SoundClassName) const
{
	if (const float* const Value = SoundClassVolumeMap.Find(SoundClassName))
	{
		return *Value;
	}

	return 0.0f;
}

void UTetrisSaveGameOption::DebugPrint(const FString& Prefix) const
{
	for (const auto& [SoundClassName, Volume] : SoundClassVolumeMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: %s: %f"), *Prefix, *SoundClassName.ToString(), Volume);
	}
}

UTetrisSaveGameOption* UTetrisSaveGameOption::LoadTetrisSaveCommonOption()
{
	if (UGameplayStatics::DoesSaveGameExist(UTetrisSaveGameOption::CommonOptionSlotName.ToString(), UTetrisSaveGameOption::UserIndex))
	{
		// Load Save Game Instance
		UTetrisSaveGameOption* const ExistingSaveGameOption = Cast<UTetrisSaveGameOption>(UGameplayStatics::LoadGameFromSlot(UTetrisSaveGameOption::CommonOptionSlotName.ToString(), UTetrisSaveGameOption::UserIndex));
		return ExistingSaveGameOption;
	}

	// Create Save Game Instance
	if (UTetrisSaveGameOption* const NewSaveGameOption = Cast<UTetrisSaveGameOption>(UGameplayStatics::CreateSaveGameObject(UTetrisSaveGameOption::StaticClass())))
	{
		NewSaveGameOption->Initialize();
		NewSaveGameOption->SaveCommonOptionSettings();
		return NewSaveGameOption;
	}

	return nullptr;
}
