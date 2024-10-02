// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PopUpWidgetBase.h"
#include "OptionPopupWidget.generated.h"

class USlider;
class UTetrisAudioManagerSubsystem;

/**
 * 
 */
UCLASS()
class TETRIS_API UOptionPopupWidget : public UPopUpWidgetBase
{
	GENERATED_BODY()

protected:
	/** UUserWidget Interface */
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	/** ~UUserWidget Interface */

	UFUNCTION()
	void OnBgmVolumeSliderValueChanged(const float NewVolume);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> BgmVolumeSlider;

	UPROPERTY()
	TObjectPtr<UTetrisAudioManagerSubsystem> AudioManager;
};
