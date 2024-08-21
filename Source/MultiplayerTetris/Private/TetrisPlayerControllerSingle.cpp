// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerControllerSingle.h"

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Kismet/GameplayStatics.h"
#include "PauseMenuWidget.h"

void ATetrisPlayerControllerSingle::SetInputModeGameOnlyAndUnPause()
{
	// Set the input mode to GameOnly
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// Unpause the game
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void ATetrisPlayerControllerSingle::SetInputModeUIOnlyAndPause()
{
	if (!PauseMenuWidget)
	{
		// Create Widget with Widget Blueprint with Name: WB_PauseMenu
		check(PauseMenuWidgetClass != nullptr);
		PauseMenuWidget = CreateWidget<UPauseMenuWidget>(this, PauseMenuWidgetClass);
		check(PauseMenuWidget != nullptr);
	}
	bShowMouseCursor = true;

	// Set the input mode to UIOnly
	const FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	PauseMenuWidget->AddToViewport();

	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ATetrisPlayerControllerSingle::BindInputActions(UEnhancedInputComponent* const EnhancedInputComponent)
{
	Super::BindInputActions(EnhancedInputComponent);

	// TogglePause
	if (TogglePauseAction)
	{
		EnhancedInputComponent->BindAction(TogglePauseAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerSingle::OnTogglePause);
	}
}

void ATetrisPlayerControllerSingle::OnTogglePause(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::OnTogglePause()"));
}
