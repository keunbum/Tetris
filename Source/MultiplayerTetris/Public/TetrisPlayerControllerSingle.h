// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisPlayerControllerInGameBase.h"

#include "TetrisPlayerControllerSingle.generated.h"

class UPauseMenuWidget;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisPlayerControllerSingle : public ATetrisPlayerControllerInGameBase
{
	GENERATED_BODY()

public:
	void SetInputModeGameOnlyAndUnPause();
	void SetInputModeUIOnlyAndPause();

protected:
	virtual void BindInputActions(UEnhancedInputComponent* const EnhancedInputComponent) override;

private:
	// Input Events
	void OnPause(const FInputActionValue& ActionValue);

private:
	// Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> PauseAction;

	/** Widget References */
	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuWidget;

	/** Widget Class Reference */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;
};
