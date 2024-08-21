// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UMainMenuWidget : public UMenuWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnStartGameClicked();

	UFUNCTION()
	void OnExitGameClicked();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> StartGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> ExitGameButton;
};
