// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisWidgetPopupOption.h"
#include "AnalogSlider.h"
#include "TetrisAudioManagerSubsystem.h"

void UTetrisWidgetPopupOption::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AudioManager = GetGameInstance()->GetSubsystem<UTetrisAudioManagerSubsystem>();

	if (BgmVolumeSlider)
	{
		BgmVolumeSlider->OnValueChanged.AddDynamic(this, &UTetrisWidgetPopupOption::OnBgmVolumeSliderValueChanged);
	}
	if (SfxVolumeSlider)
	{
		SfxVolumeSlider->OnValueChanged.AddDynamic(this, &UTetrisWidgetPopupOption::OnSfxVolumeSliderValueChanged);
	}
}

void UTetrisWidgetPopupOption::NativeConstruct()
{
	Super::NativeConstruct();

	if (BgmVolumeSlider)
	{
		const float BgmVolume = AudioManager->GetBgmVolume();
		BgmVolumeSlider->SetValue(BgmVolume);
	}

	if (SfxVolumeSlider)
	{
		const float SfxVolume = AudioManager->GetSfxVolume();
		SfxVolumeSlider->SetValue(SfxVolume);
	}
}

void UTetrisWidgetPopupOption::NativeDestruct()
{
	if (AudioManager)
	{
		AudioManager->SaveSettings();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UTetrisWidgetPopupOption::NativeDestruct() - Failed to get AudioManager"));
	}
}

void UTetrisWidgetPopupOption::OnBgmVolumeSliderValueChanged(const float NewVolume)
{
	if (AudioManager)
	{
		AudioManager->SetBgmVolume(NewVolume);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UTetrisWidgetPopupOption::OnBgmVolumeSliderValueChanged() - Failed to get AudioManager"));
	}
}

void UTetrisWidgetPopupOption::OnSfxVolumeSliderValueChanged(const float NewVolume)
{
	if (AudioManager)
	{
		AudioManager->SetSfxVolume(NewVolume);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UTetrisWidgetPopupOption::OnSfxVolumeSliderValueChanged() - Failed to get AudioManager"));
	}
}

