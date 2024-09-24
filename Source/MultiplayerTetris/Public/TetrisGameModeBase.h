// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetrisGameModeBase.generated.h"

struct FInputModeDataBase;
class UAudioComponent;

/**
 * 
 */
UCLASS(Abstract)
class MULTIPLAYERTETRIS_API ATetrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UAudioComponent* CreateAudioComponent(const FName& CuePath) const;

protected:
	virtual void BeginPlay() override;

	void InternalSetInputMode(const FInputModeDataBase& InputModeData);
	virtual void Initialize();
	virtual void InitializeDefaultEffect() {}

private:
	// Declare PURE_VIRTUAL functions
	virtual void SetInputMode() PURE_VIRTUAL(ATetrisGameModeBase::SetInputMode);

public:
	/** Player */
	static constexpr int32 PlayerIndex = 0;

protected:
	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMComponent;
};
