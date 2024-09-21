// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PopUpWidgetBase.h"
#include "OptionPopUpWidget.generated.h"

class UButtonBase;
class UAudioVolumeSlider;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UOptionPopUpWidget : public UPopUpWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAudioVolumeSlider> BGMVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButtonBase> CloseButton;
};
