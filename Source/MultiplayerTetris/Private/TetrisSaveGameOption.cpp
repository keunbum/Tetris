// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisSaveGameOption.h"
#include "Kismet/GameplayStatics.h"

const FName UTetrisSaveGameOption::SlotName(TEXT("CommonOptionSaveSlot"));

void UTetrisSaveGameOption::Initialize()
{
	MainMenuBGMVolume = 1.0f;
}

void UTetrisSaveGameOption::SaveSetting()
{
	UGameplayStatics::SaveGameToSlot(this, UTetrisSaveGameOption::SlotName.ToString(), UTetrisSaveGameOption::UserIndex);
}

UTetrisSaveGameOption* UTetrisSaveGameOption::LoadTetrisSaveGame()
{
	UTetrisSaveGameOption* SaveGameOption = Cast<UTetrisSaveGameOption>(UGameplayStatics::LoadGameFromSlot(UTetrisSaveGameOption::SlotName.ToString(), UTetrisSaveGameOption::UserIndex));
	if (!SaveGameOption)
	{
		SaveGameOption = Cast<UTetrisSaveGameOption>(UGameplayStatics::CreateSaveGameObject(UTetrisSaveGameOption::StaticClass()));
		SaveGameOption->Initialize();
		SaveGameOption->SaveSetting();
	}

	return SaveGameOption;
}
