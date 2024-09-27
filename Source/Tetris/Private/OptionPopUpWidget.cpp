// Copyright Ryu KeunBeom. All Rights Reserved.

#include "OptionPopUpWidget.h"
#include "Components/Slider.h"
#include "TetrisAudioInstanceSubsystem.h"


void UOptionPopUpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!AudioInstanceSubsystem)
	{
		// Get GameInstance Subsystem
		AudioInstanceSubsystem = GetGameInstance()->GetSubsystem<UTetrisAudioInstanceSubsystem>();
	}

	if (BgmVolumeSlider)
	{
		if (!BgmVolumeSlider->OnValueChanged.IsBound())
		{
			BgmVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionPopUpWidget::OnBgmVolumeSliderValueChanged);
		}
		BgmVolumeSlider->SetValue(AudioInstanceSubsystem->GetSoundClassVolumeByName(UTetrisAudioInstanceSubsystem::BgmSoundClassName));
	}
}

void UOptionPopUpWidget::PrepareClose()
{
	Super::PrepareClose();
}

void UOptionPopUpWidget::OnBgmVolumeSliderValueChanged(const float NewVolume)
{
	if (AudioInstanceSubsystem)
	{
		AudioInstanceSubsystem->SetSoundClassVolumeByName(UTetrisAudioInstanceSubsystem::BgmSoundClassName, NewVolume);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionPopUpWidget::OnBgmVolumeSliderValueChanged() - Failed to get AudioInstanceSubsystem"));
	}
}
