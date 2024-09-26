// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "GameOverMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UGameOverMenuWidget : public UMenuWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

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
