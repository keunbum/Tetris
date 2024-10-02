// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisWidgetBase.h"
#include "PopupWidgetBase.generated.h"

class UButtonBase;

/**
 * 
 */
UCLASS(Abstract)
class TETRIS_API UPopupWidgetBase : public UTetrisWidgetBase
{
	GENERATED_BODY()

protected:
	/** UUserWidget Interface */
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	/** ~UUserWidget Interface */

private:
	UFUNCTION()
	void OnCloseClicked();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButtonBase> CloseButton;
};
