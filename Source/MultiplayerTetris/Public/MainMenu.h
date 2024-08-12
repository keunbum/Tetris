// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UMenuButton;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void SetInitialFocus();
	void SetInitialMenuButtonFocus();
	void SetMenuButtonFocus(const int32 NewFocusedButtonIndex);
	void MoveMenuButtonFocus(const int32 Delta);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> StartGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> ExitGameButton;

	UPROPERTY()
	TArray<TObjectPtr<UMenuButton>> MenuButtons;

	UPROPERTY(VisibleAnywhere)
	int32 FocusedButtonIndex = 0;
};
