// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisGameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API AMainMenuGameMode : public ATetrisGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	virtual void Initialize() override;
	virtual void SetInputMode() override;
	virtual void LoadSetting() override;
	virtual bool LoadSaveGameInstance() override;
	virtual void LoadSoundSetting() override;

public:
	// Level Name
	static const FName MainMenuLevelName;
	static const FName BGMCuePath;
};
