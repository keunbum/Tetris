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
	UTetrisSaveGameOption* SaveGame = Cast<UTetrisSaveGameOption>(UGameplayStatics::LoadGameFromSlot(UTetrisSaveGameOption::SlotName.ToString(), UTetrisSaveGameOption::UserIndex));
	if (!SaveGame)
	{
		SaveGame = Cast<UTetrisSaveGameOption>(UGameplayStatics::CreateSaveGameObject(UTetrisSaveGameOption::StaticClass()));
		SaveGame->Initialize();
		SaveGame->SaveSetting();
	}

	return SaveGame;
}
