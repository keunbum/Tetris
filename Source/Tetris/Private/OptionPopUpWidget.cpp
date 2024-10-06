// Copyright Ryu KeunBeom. All Rights Reserved.

#include "OptionPopupWidget.h"
#include "AnalogSlider.h"
#include "TetrisAudioManagerSubsystem.h"


void UOptionPopupWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AudioManager = GetGameInstance()->GetSubsystem<UTetrisAudioManagerSubsystem>();

	if (BgmVolumeSlider)
	{
		BgmVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionPopupWidget::OnBgmVolumeSliderValueChanged);
	}
}

void UOptionPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BgmVolumeSlider)
	{
		const float BgmVolume = AudioManager->GetBgmVolume();
		BgmVolumeSlider->SetValue(BgmVolume);
	}
}

void UOptionPopupWidget::NativeDestruct()
{
	if (AudioManager)
	{
		AudioManager->SaveSettings();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionPopupWidget::NativeDestruct() - Failed to get AudioManager"));
	}
}

void UOptionPopupWidget::OnBgmVolumeSliderValueChanged(const float NewVolume)
{
	if (AudioManager)
	{
		AudioManager->SetBgmVolume(NewVolume);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionPopupWidget::OnBgmVolumeSliderValueChanged() - Failed to get AudioManager"));
	}
}
