// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisPlayerControllerIngame.h"

#include "TetrisPlayerControllerIngameSingle.generated.h"

class UTetrisWidgetMenuPause;
class UTetrisWidgetMenuGameover;

/**
 * 
 */
UCLASS()
class TETRIS_API ATetrisPlayerControllerIngameSingle : public ATetrisPlayerControllerIngame
{
	GENERATED_BODY()

public:
	void SetInputModeGameOnlyAndUnPause();
	void SetInputModeUIOnlyAndPause();
	void SetInputModeUIOnlyAndGameOver();

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
	TObjectPtr<UTetrisWidgetMenuPause> PauseMenuWidget;

	UPROPERTY()
	TObjectPtr<UTetrisWidgetMenuGameover> GameOverMenuWidget;

	/** Widget Class Reference */
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<UTetrisWidgetMenuPause> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<UTetrisWidgetMenuGameover> GameOverMenuWidgetClass;
};
