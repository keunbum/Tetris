// Copyright Ryu KeunBeom. All Rights Reserved.

#include "OptionPopUpWidget.h"
#include "Components/Slider.h"
#include "TetrisAudioManagerSubsystem.h"


void UOptionPopUpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!AudioManager)
	{
		// Get GameInstance Subsystem
		AudioManager = GetGameInstance()->GetSubsystem<UTetrisAudioManagerSubsystem>();
	}

	if (BgmVolumeSlider)
	{
		if (!BgmVolumeSlider->OnValueChanged.IsBound())
		{
			BgmVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionPopUpWidget::OnBgmVolumeSliderValueChanged);
		}
		const float BgmVolume = AudioManager->GetBgmVolume();
		BgmVolumeSlider->SetValue(BgmVolume);
	}
}

void UOptionPopUpWidget::PrepareClose()
{
	Super::PrepareClose();

	if (AudioManager)
	{
		AudioManager->SaveSettings();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionPopUpWidget::PrepareClose() - Failed to get AudioManager"));
	}
}

void UOptionPopUpWidget::OnBgmVolumeSliderValueChanged(const float NewVolume)
{
	if (AudioManager)
	{
		AudioManager->SetBgmVolume(NewVolume);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionPopUpWidget::OnBgmVolumeSliderValueChanged() - Failed to get AudioManager"));
	}
}
