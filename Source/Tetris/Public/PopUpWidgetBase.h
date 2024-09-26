// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopUpWidgetBase.generated.h"

class UButtonBase;

/**
 * 
 */
UCLASS()
class TETRIS_API UPopUpWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void PrepareClose() {}
	
private:
	UFUNCTION()
	void OnCloseClicked();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButtonBase> CloseButton;
};
