// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UMainMenu : public UMenuBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	void SetInitialFocus();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> StartGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> ExitGameButton;
};
