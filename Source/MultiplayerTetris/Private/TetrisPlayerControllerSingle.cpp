// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerControllerSingle.h"

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"

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
