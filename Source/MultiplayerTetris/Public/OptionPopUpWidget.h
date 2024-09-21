// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PopUpWidgetBase.h"
#include "OptionPopUpWidget.generated.h"

class UAudioVolumeSlider;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UOptionPopUpWidget : public UPopUpWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAudioVolumeSlider> BGMVolumeSlider;
};
