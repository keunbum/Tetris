// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "PauseMenuWidget.generated.h"

class ATetrisPlayerControllerSingle;

/**
 * 
 */
UCLASS()
class TETRIS_API UPauseMenuWidget : public UMenuWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

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

	UPROPERTY()
	TObjectPtr<ATetrisPlayerControllerSingle> TetrisPlayerController;
};
