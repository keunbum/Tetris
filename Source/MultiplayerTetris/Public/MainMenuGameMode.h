// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisGameModeBase.h"
#include "MainMenuGameMode.generated.h"

class USoundCue;
class UAudioComponent;
class UTetrisSaveGameOption;

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
	void LoadSoundSetting();
	UAudioComponent* CreateAudioComponent(const TCHAR* CuePath) const;

	static void SetAudioComponentVolume(UAudioComponent* const AudioComponent, const float Volume);

public:
	// Level Name
	static const FName MainMenuLevelName;
	static const FName BGMCuePath;

private:
	UPROPERTY()
	TObjectPtr<UTetrisSaveGameOption> TetrisSaveGameOption;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMComponent;
};
