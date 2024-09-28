// Copyright Ryu KeunBeom. All Rights Reserved.

#include "OptionPopUpWidget.h"
#include "Components/Slider.h"
#include "TetrisAudioManagerSubsystem.h"


void UOptionPopUpWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AudioManager = GetGameInstance()->GetSubsystem<UTetrisAudioManagerSubsystem>();

	if (BgmVolumeSlider)
	{
		BgmVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionPopUpWidget::OnBgmVolumeSliderValueChanged);
	}
}

void UOptionPopUpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BgmVolumeSlider)
	{
		const float BgmVolume = AudioManager->GetBgmVolume();
		BgmVolumeSlider->SetValue(BgmVolume);
	}
}

void UOptionPopUpWidget::NativeDestruct()
{
	if (AudioManager)
	{
		AudioManager->SaveSettings();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionPopUpWidget::NativeDestruct() - Failed to get AudioManager"));
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
