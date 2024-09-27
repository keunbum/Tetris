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
	/** ~AActor */

	void InternalSetInputMode(const FInputModeDataBase& InputModeData);
	virtual void Initialize();
	virtual void InitializeDefaultEffect();

private:
	// Declare PURE_VIRTUAL functions
	virtual void SetInputMode() PURE_VIRTUAL(ATetrisGameModeBase::SetInputMode);

public:
	/** Player */
	static constexpr int32 PlayerIndex = 0;

protected:
	/** Audio */
	// BGM
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundCue> BGMCue;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	float BGMFadeInTime = 0.f;
};
