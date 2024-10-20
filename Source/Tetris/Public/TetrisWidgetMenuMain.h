// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisWidgetMenuBase.h"
#include "TetrisWidgetMenuMain.generated.h"

class UTetrisWidgetPopupOption;

/**
 * 
 */
UCLASS()
class TETRIS_API UTetrisWidgetMenuMain : public UTetrisWidgetMenuBase
{
	GENERATED_BODY()

protected:
	/** UUserWidget Interface */
	virtual void NativeOnInitialized() override;
	/** ~UUserWidget Interface */

private:
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnOptionClicked();

	UFUNCTION()
	void OnExitClicked();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> OptionButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> ExitButton;

	/** Widget References */
	UPROPERTY()
	TObjectPtr<UTetrisWidgetPopupOption> WidgetPopupOption;

	/** Widget Class References */
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<UTetrisWidgetPopupOption> WidgetPopupOptionClass;
};
