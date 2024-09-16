// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetrisGameModeBase.generated.h"

struct FInputModeDataBase;
class UAudioComponent;
class UTetrisSaveGameOption;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UAudioComponent* CreateAudioComponent(const FName& CuePath) const;

	// static methods
	static void SetAudioComponentVolume(UAudioComponent* const AudioComponent, const float Volume);

protected:
	virtual void BeginPlay() override;

	void InternalSetInputMode(const FInputModeDataBase& InputModeData);
	virtual void Initialize();
	virtual void LoadSetting();

private:
	// Declare PURE_VIRTUAL functions
	virtual bool LoadSaveGameInstance() PURE_VIRTUAL(ATetrisGameModeBase::LoadSaveGameInstance, return false;);
	virtual void LoadSoundSetting() PURE_VIRTUAL(ATetrisGameModeBase::LoadSoundSetting);
	virtual void SetInputMode() PURE_VIRTUAL(ATetrisGameModeBase::SetInputMode);

public:
	/** Player */
	static constexpr int32 PlayerIndex = 0;

protected:
	UPROPERTY()
	TObjectPtr<UTetrisSaveGameOption> TetrisSaveGameOption;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMComponent;
};
