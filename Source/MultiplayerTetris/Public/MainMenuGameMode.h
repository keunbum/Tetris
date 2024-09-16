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
	void ToggleBGM(const bool bIsBGMOn);

private:
	void Initialize();
	void SetInputMode();
	void LoadSetting();
	void LoadSoundSetting();
	UAudioComponent* CreateAudioComponent(const TCHAR* CuePath) const;

	static void SetAudioComponentVolume(UAudioComponent* const AudioComponent, const float Volume);

public:
	// Level Name
	static const FName MainMenuLevelName;
	static const FName BGMCuePath;

private:
	UPROPERTY()
	TObjectPtr<UTetrisSaveGame> TetrisSaveGame;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMComponent;
};
