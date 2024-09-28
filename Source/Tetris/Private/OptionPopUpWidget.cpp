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
		const float BgmVolume = AudioInstanceSubsystem->GetBgmVolume();
		BgmVolumeSlider->SetValue(BgmVolume);
	}
}

void UOptionPopUpWidget::PrepareClose()
{
	Super::PrepareClose();

	if (AudioInstanceSubsystem)
	{
		AudioInstanceSubsystem->SaveSettings();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionPopUpWidget::PrepareClose() - Failed to get AudioInstanceSubsystem"));
	}
}

void UOptionPopUpWidget::OnBgmVolumeSliderValueChanged(const float NewVolume)
{
	if (AudioInstanceSubsystem)
	{
		AudioInstanceSubsystem->SetBgmVolume(NewVolume);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionPopUpWidget::OnBgmVolumeSliderValueChanged() - Failed to get AudioInstanceSubsystem"));
	}
}
