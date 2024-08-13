// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisPlayerControllerInGameBase.h"

#include "TetrisPlayerControllerSingle.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisPlayerControllerSingle : public ATetrisPlayerControllerInGameBase
{
	GENERATED_BODY()

protected:
	virtual void BindInputActions(UEnhancedInputComponent* const EnhancedInputComponent) override;

private:
	// Input callback methods
	void OnTogglePause(const FInputActionValue& ActionValue);

private:
	// Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> TogglePauseAction;
};
