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

	if (BGMVolumeSlider)
	{
		if (!BGMVolumeSlider->OnValueChanged.IsBound())
		{
			BGMVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionPopUpWidget::OnBGMVolumeSliderValueChanged);
		}
		BGMVolumeSlider->SetValue(AudioInstanceSubsystem->GetSoundClassVolumeByName(UTetrisAudioInstanceSubsystem::BgmSoundClassName));
	}
}

void UOptionPopUpWidget::PrepareClose()
{
	Super::PrepareClose();
}

void UOptionPopUpWidget::OnBGMVolumeSliderValueChanged(const float NewVolume)
{
	if (AudioInstanceSubsystem)
	{
		AudioInstanceSubsystem->SetSoundClassVolumeByName(UTetrisAudioInstanceSubsystem::BgmSoundClassName, NewVolume);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionPopUpWidget::OnBGMVolumeSliderValueChanged() - Failed to get AudioInstanceSubsystem"));
	}
}
