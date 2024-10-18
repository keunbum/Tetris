// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisWidgetPopupBase.h"
#include "OptionPopupWidget.generated.h"

class UAnalogSlider;
class UTetrisAudioManagerSubsystem;

/**
 * 
 */
UCLASS()
class TETRIS_API UOptionPopupWidget : public UTetrisWidgetPopupBase
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

	UFUNCTION()
	void OnSfxVolumeSliderValueChanged(const float NewVolume);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAnalogSlider> BgmVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAnalogSlider> SfxVolumeSlider;

	UPROPERTY()
	TObjectPtr<UTetrisAudioManagerSubsystem> AudioManager;
};
