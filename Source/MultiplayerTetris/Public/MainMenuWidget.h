// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "MainMenuWidget.generated.h"

class UOptionPopUpWidget;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UMainMenuWidget : public UMenuWidgetBase
{
	GENERATED_BODY()

public:
	UMainMenuWidget();

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnOptionClicked();

	UFUNCTION()
	void OnExitClicked();

public:
	static const FName OptionPopUpWidgetPath;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> OptionButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> ExitButton;

	/** Widget References */
	UPROPERTY()
	TObjectPtr<UOptionPopUpWidget> OptionPopUpWidget;

	/** Widget Class References */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOptionPopUpWidget> OptionPopUpWidgetClass;
};
