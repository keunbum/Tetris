// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PopUpWidgetBase.h"
#include "OptionPopUpWidget.generated.h"

class USlider;
class UTetrisAudioInstanceSubsystem;

/**
 * 
 */
UCLASS()
class TETRIS_API UOptionPopUpWidget : public UPopUpWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void PrepareClose() override;

	UFUNCTION()
	void OnBgmVolumeSliderValueChanged(const float NewVolume);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> BgmVolumeSlider;

	UPROPERTY()
	TObjectPtr<UTetrisAudioInstanceSubsystem> AudioInstanceSubsystem;
};
