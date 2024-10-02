// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "TetrisAudioManagerSubsystem.h"

UAudioComponent* ATetrisGameModeBase::CreateAudioComponent(USoundCue* const SoundCue) const
{
	if (SoundCue)
	{
		if (UAudioComponent* const AudioComponent = UGameplayStatics::CreateSound2D(GetWorld(), SoundCue))
		{
			return AudioComponent;
		}

		UE_LOG(LogTemp, Error, TEXT("ATetrisGameModeBase::CreateAudioComponent() - Failed to load SoundCue"));
		return nullptr;
	}

	UE_LOG(LogTemp, Error, TEXT("ATetrisGameModeBase::CreateAudioComponent() - SoundCue is nullptr"));
	return nullptr;
}

void ATetrisGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ATetrisGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Uninitialize();

	Super::EndPlay(EndPlayReason);
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
	InitializeEffect();
}

void ATetrisGameModeBase::InitializeEffect()
{
	if (UTetrisAudioManagerSubsystem* const AudioManager = GetGameInstance()->GetSubsystem<UTetrisAudioManagerSubsystem>())
	{
		BgmComponent = CreateAudioComponent(BgmCue);
		if (BgmComponent)
		{
			BgmComponent->FadeIn(BgmFadeInTime);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ATetrisGameModeBase::InitializeEffect() - Failed to get AudioManager"));
	}
}

void ATetrisGameModeBase::Uninitialize()
{
	UninitializeEffect();
}

void ATetrisGameModeBase::UninitializeEffect()
{
	if (BgmComponent)
	{
		BgmComponent->FadeOut(BgmFadeOutTime, 0.f /* FadeVolumeLevel */);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ATetrisGameModeBase::UninitializeEffect() - Failed to get BgmComponent"));
	}
}
