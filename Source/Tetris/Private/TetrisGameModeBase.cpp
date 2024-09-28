// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "TetrisAudioManagerSubsystem.h"

UAudioComponent* ATetrisGameModeBase::CreateAudioComponent(USoundCue* const SoundCue) const
{
	if (UAudioComponent* const AudioComponent = UGameplayStatics::CreateSound2D(GetWorld(), SoundCue))
	{
		return AudioComponent;
	}

	UE_LOG(LogTemp, Error, TEXT("ATetrisGameModeBase::CreateAudioComponent() - Failed to load SoundCue"));
	return nullptr;
}

void ATetrisGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ATetrisGameModeBase::InternalSetInputMode(const FInputModeDataBase& InputMode)
{
	if (APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex))
	{
		PlayerController->SetInputMode(InputMode);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ATetrisGameModeBase::InternalSetInputMode() - Failed to get PlayerController"));
	}
}

void ATetrisGameModeBase::Initialize()
{
	SetInputMode();
	InitializeDefaultEffect();
}

void ATetrisGameModeBase::InitializeDefaultEffect()
{
	if (UTetrisAudioManagerSubsystem* const AudioInstanceSubsystem = GetGameInstance()->GetSubsystem<UTetrisAudioManagerSubsystem>())
	{
		if (BgmCue)
		{
			BgmComponent = CreateAudioComponent(BgmCue);
			if (BgmComponent)
			{
				const float BgmVolume = AudioInstanceSubsystem->GetSoundClassVolumeByName(UTetrisAudioManagerSubsystem::BgmSoundClassName);
				BgmComponent->FadeIn(BgmFadeInTime, BgmVolume);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ATetrisGameModeBase::InitializeDefaultEffect() - Failed to get AudioInstanceSubsystem"));
	}
}
