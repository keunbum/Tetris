// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "TetrisPlayerControllerSingle.h"
#include "TetrisGameModeBase.h"

const FName AMainMenuGameMode::MainMenuLevelName(TEXT("MainMenuLevel"));

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ATetrisPlayerControllerSingle* const TetrisPlayerController = Cast<ATetrisPlayerControllerSingle>(UGameplayStatics::GetPlayerController(GetWorld(), ATetrisGameModeBase::PlayerIndex)))
	{
		TetrisPlayerController->bShowMouseCursor = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuGameMode::BeginPlay() - Failed to get TetrisPlayerController"));
	}
}
