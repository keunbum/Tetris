// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerControllerSingle.h"

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Kismet/GameplayStatics.h"

void ATetrisPlayerControllerSingle::SetInputModeGameOnlyAndUnPause()
{
	// Set the input mode to GameOnly
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// Unpause the game
	UGameplayStatics::SetGamePaused(GetWorld(), false);
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
