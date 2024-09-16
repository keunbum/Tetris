// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisSaveGameOption.h"
#include "Kismet/GameplayStatics.h"

const FName UTetrisSaveGameOption::SlotName(TEXT("CommonOptionSaveSlot"));

void UTetrisSaveGameOption::Initialize()
{
	BGMVolume = 1.0f;
}

void UTetrisSaveGameOption::SaveSetting()
{
	UGameplayStatics::SaveGameToSlot(this, UTetrisSaveGameOption::SlotName.ToString(), UTetrisSaveGameOption::UserIndex);
}

UTetrisSaveGameOption* UTetrisSaveGameOption::LoadTetrisSaveGameOptionInstance()
{
	if (UTetrisSaveGameOption* const ExistingSaveGameOption = Cast<UTetrisSaveGameOption>(UGameplayStatics::LoadGameFromSlot(UTetrisSaveGameOption::SlotName.ToString(), UTetrisSaveGameOption::UserIndex)))
	{
		return ExistingSaveGameOption;
	}

	if (UTetrisSaveGameOption* const NewSaveGameOption = Cast<UTetrisSaveGameOption>(UGameplayStatics::CreateSaveGameObject(UTetrisSaveGameOption::StaticClass())))
	{
		NewSaveGameOption->Initialize();
		NewSaveGameOption->SaveSetting();
		return NewSaveGameOption;
	}

	UE_LOG(LogTemp, Error, TEXT("UTetrisSaveGameOption::LoadTetrisSaveGameOptionInstance() - Failed to create SaveGameOption"));
	return nullptr;
}
