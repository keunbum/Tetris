// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBase.generated.h"

class UMenuButton;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeMenuButtons(const TArray<UMenuButton*>& InMenuButtons);

	bool IsNoButtonFocused() const { return FocusedButtonIndex == InvalidButtonIndex; }
protected:
	void SetMenuButtonFocus(const int32 NewFocusedButtonIndex);
	void MoveMenuButtonFocus(const int32 Delta);

protected:
	static constexpr int32 InvalidButtonIndex = -1;

	UPROPERTY()
	TArray<TObjectPtr<UMenuButton>> MenuButtons;

	UPROPERTY(VisibleAnywhere)
	int32 FocusedButtonIndex = 0;
};
