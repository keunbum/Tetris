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
protected:
	static constexpr int32 InvalidButtonIndex = -1;

	UPROPERTY()
	TArray<TObjectPtr<UMenuButton>> MenuButtons;
};
