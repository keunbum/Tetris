// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisSaveGameOption.h"
#include "Kismet/GameplayStatics.h"

const FName UTetrisSaveGameOption::CommonOptionSlotName(TEXT("CommonOptionSaveSlot"));

void UTetrisSaveGameOption::Initialize()
{
	MainSoundClassVolume = 1.0f;
	BGMSoundClassVolume = 1.0f;
}

void UTetrisSaveGameOption::SaveCommonOptionSettings()
{
	UGameplayStatics::SaveGameToSlot(this, UTetrisSaveGameOption::CommonOptionSlotName.ToString(), UTetrisSaveGameOption::UserIndex);
}

void UTetrisSaveGameOption::DebugPrint(const FString& Prefix) const
{
	UE_LOG(LogTemp, Warning, TEXT("%s: MainSoundClassVolume: %f"), *Prefix, MainSoundClassVolume);
	UE_LOG(LogTemp, Warning, TEXT("%s: BGMSoundClassVolume: %f"), *Prefix, BGMSoundClassVolume);
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
