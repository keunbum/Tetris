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
		check(AudioInstanceSubsystem != nullptr);
	}

	if (BGMVolumeSlider)
	{
		if (!BGMVolumeSlider->OnValueChanged.IsBound())
		{
			BGMVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionPopUpWidget::OnBGMVolumeChanged);
		}
		BGMVolumeSlider->SetValue(AudioInstanceSubsystem->GetBGMSoundClassVolume());
	}
}

void UOptionPopUpWidget::PrepareClose()
{
	Super::PrepareClose();
}

void UOptionPopUpWidget::OnBGMVolumeChanged(const float NewVolume)
{
	if (AudioInstanceSubsystem)
	{
		AudioInstanceSubsystem->SetBGMSoundClassVolume(NewVolume);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionPopUpWidget::OnBGMVolumeChanged() - Failed to get AudioInstanceSubsystem"));
	}
}
