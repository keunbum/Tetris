// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetrisGameModeBase.generated.h"

struct FInputModeDataBase;
class UAudioComponent;
class USoundCue;

/**
 * 
 */
UCLASS(Abstract)
class TETRIS_API ATetrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UAudioComponent* CreateAudioComponent(USoundCue* const SoundCue) const;

protected:
	/** AActor */
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	/** ~AActor */

	void InternalSetInputMode(const FInputModeDataBase& InputModeData);
	virtual void Initialize();
	virtual void InitializeEffect();
	virtual void Uninitialize();
	virtual void UninitializeEffect();

private:
	// Declare PURE_VIRTUAL functions
	virtual void SetInputMode() PURE_VIRTUAL(ATetrisGameModeBase::SetInputMode);

public:
	/** Player */
	static constexpr int32 PlayerIndex = 0;

protected:
	/** Audio */
	// Bgm
	UPROPERTY()
	TObjectPtr<UAudioComponent> BgmComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundCue> BgmCue;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	float BgmFadeInTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	float BgmFadeOutTime = 0.f;
};
