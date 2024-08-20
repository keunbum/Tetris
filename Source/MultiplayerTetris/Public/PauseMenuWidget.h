// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UPauseMenuWidget : public UMenuWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void SetInitialFocus() override;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnResumeClicked();
	UFUNCTION()
	void OnRestartClicked();
	UFUNCTION()
	void OnExitClicked();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> ResumeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> ExitButton;
};
