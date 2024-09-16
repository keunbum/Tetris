// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "TetrisPlayerControllerSingle.h"
#include "TetrisGameModeBase.h"

const FName AMainMenuGameMode::MainMenuLevelName(TEXT("MainMenuLevel"));

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode();
}

void AMainMenuGameMode::SetInputMode()
{
	if (ATetrisPlayerControllerSingle* const TetrisPlayerController = Cast<ATetrisPlayerControllerSingle>(UGameplayStatics::GetPlayerController(GetWorld(), ATetrisGameModeBase::PlayerIndex)))
	{
		// Set the input mode to UIOnly
		const FInputModeUIOnly InputMode;
		TetrisPlayerController->SetInputMode(InputMode);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuGameMode::BeginPlay() - Failed to get TetrisPlayerController"));
	}
}
