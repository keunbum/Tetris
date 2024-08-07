// Copyright Ryu KeunBeom. All Rights Reserved.

#include "MainMenuGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "TetrisPlayerController.h"
#include "TetrisGameModeBase.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ATetrisPlayerController* const TetrisPlayerController = Cast<ATetrisPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), ATetrisGameModeBase::PlayerIndex)))
	{
		TetrisPlayerController->bShowMouseCursor = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuGameMode::BeginPlay() - Failed to get TetrisPlayerController"));
	}
}
