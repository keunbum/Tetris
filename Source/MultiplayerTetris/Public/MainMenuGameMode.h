// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

class USoundCue;
class UAudioComponent;
class UTetrisSaveGame;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	// Level Name
	static const FName MainMenuLevelName;

	void ToggleBGM(const bool bIsBGMOn);

private:
	void Initialize();
	void SetInputMode();
	void LoadSetting();
	void LoadSoundSetting();
	void PlayEffect();
	void PlaySoundEffect();
	void SaveSetting(const FString& SlotName);

private:
	UPROPERTY()
	TObjectPtr<UTetrisSaveGame> TetrisSaveGame;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMComponent;
};
