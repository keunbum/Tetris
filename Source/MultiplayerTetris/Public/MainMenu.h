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
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void SetInitialFocus();
	void SetInitialMenuButtonFocus();
	void MoveMenuButtonFocus(const int32 Delta);
	void SetMenuButtonFocus(const int32 NewFocusedButtonIndex);

protected:
	static constexpr int32 InitialFocusedButtonIndex = 0;
	
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
