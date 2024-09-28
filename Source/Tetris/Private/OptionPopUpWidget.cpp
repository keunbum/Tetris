// Copyright Ryu KeunBeom. All Rights Reserved.

#include "OptionPopUpWidget.h"
#include "Components/Slider.h"
#include "TetrisAudioManagerSubsystem.h"


void UOptionPopUpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!AudioInstanceSubsystem)
	{
		// Get GameInstance Subsystem
		AudioInstanceSubsystem = GetGameInstance()->GetSubsystem<UTetrisAudioManagerSubsystem>();
	}

	if (BgmVolumeSlider)
	{
		if (!BgmVolumeSlider->OnValueChanged.IsBound())
		{
			BgmVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionPopUpWidget::OnBgmVolumeSliderValueChanged);
		}
		BgmVolumeSlider->SetValue(AudioInstanceSubsystem->GetSoundClassVolumeByName(UTetrisAudioManagerSubsystem::BgmSoundClassName));
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
		AudioInstanceSubsystem->SetSoundClassVolumeByName(UTetrisAudioManagerSubsystem::BgmSoundClassName, NewVolume);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionPopUpWidget::OnBgmVolumeSliderValueChanged() - Failed to get AudioInstanceSubsystem"));
	}
}
