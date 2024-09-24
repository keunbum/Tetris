// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

UAudioComponent* ATetrisGameModeBase::CreateAudioComponent(const FName& CuePath) const
{
	if (USoundCue* const SoundCue = LoadObject<USoundCue>(nullptr, *CuePath.ToString()))
	{
		if (UAudioComponent* const AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), SoundCue))
		{
			return AudioComponent;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ATetrisGameModeBase::CreateAudioComponent() - Failed to spawn AudioComponent"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ATetrisGameModeBase::CreateAudioComponent() - Failed to load SoundCue"));
	}

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
	InitDefaultEffect();
}
