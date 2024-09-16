// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ATetrisGameModeBase::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisGameModeBase::BeginPlay()"));
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
	LoadSetting();
}
