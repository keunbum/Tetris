// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisWidgetMenuBase.h"
#include "TetrisWidgetMenuGameover.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UTetrisWidgetMenuGameover : public UTetrisWidgetMenuBase
{
	GENERATED_BODY()
	
protected:
	/** UUserWidget Interface */
	virtual void NativeOnInitialized() override;
	/** ~UUserWidget Interface */

private:
	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnHomeClicked();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> HomeButton;
};
