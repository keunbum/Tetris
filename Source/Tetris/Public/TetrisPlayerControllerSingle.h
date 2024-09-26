// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisPlayerControllerInGameBase.h"

#include "TetrisPlayerControllerSingle.generated.h"

class UPauseMenuWidget;
class UGameOverMenuWidget;

/**
 * 
 */
UCLASS()
class TETRIS_API ATetrisPlayerControllerSingle : public ATetrisPlayerControllerInGameBase
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
	TObjectPtr<UPauseMenuWidget> PauseMenuWidget;

	UPROPERTY()
	TObjectPtr<UGameOverMenuWidget> GameOverMenuWidget;

	/** Widget Class Reference */
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<UGameOverMenuWidget> GameOverMenuWidgetClass;
};