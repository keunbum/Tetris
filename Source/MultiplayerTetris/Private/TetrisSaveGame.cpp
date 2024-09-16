// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisSaveGame.h"
#include "Kismet/GameplayStatics.h"

const FName UTetrisSaveGame::SlotName(TEXT("CommonOptionSaveSlot"));

void UTetrisSaveGame::Initialize()
{
	MainMenuBGMVolume = 1.0f;
}

void UTetrisSaveGame::SaveSetting()
{
	UGameplayStatics::SaveGameToSlot(this, UTetrisSaveGame::SlotName.ToString(), UTetrisSaveGame::UserIndex);
}

UTetrisSaveGame* UTetrisSaveGame::LoadTetrisSaveGame()
{
	UTetrisSaveGame* SaveGame = Cast<UTetrisSaveGame>(UGameplayStatics::LoadGameFromSlot(UTetrisSaveGame::SlotName.ToString(), UTetrisSaveGame::UserIndex));
	if (!SaveGame)
	{
		SaveGame = Cast<UTetrisSaveGame>(UGameplayStatics::CreateSaveGameObject(UTetrisSaveGame::StaticClass()));
		SaveGame->Initialize();
	}

	return SaveGame;
}
