// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerControllerSingle.h"

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Kismet/GameplayStatics.h"
#include "PauseMenuWidget.h"

void ATetrisPlayerControllerSingle::SetInputModeGameOnlyAndUnPause()
{
	// Set the input mode to GameOnly
	const FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// Remove the pause menu widget
	PauseMenuWidget->RemoveFromParent();

	// Set focus to the game viewport
	FSlateApplication::Get().SetAllUserFocusToGameViewport();

	// Unpause the game
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void ATetrisPlayerControllerSingle::SetInputModeUIOnlyAndPause()
{
	if (!PauseMenuWidget)
	{
		check(PauseMenuWidgetClass != nullptr);
		PauseMenuWidget = CreateWidget<UPauseMenuWidget>(this, PauseMenuWidgetClass);
		check(PauseMenuWidget != nullptr);
	}

	// Set the input mode to UIOnly
	const FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	check(!PauseMenuWidget->IsInViewport());
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
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::OnTogglePause()"));
	const bool bIsPausing = UGameplayStatics::IsGamePaused(GetWorld());
	const bool bWillBePaused = !bIsPausing;
	if (bWillBePaused)
	{
		SetInputModeUIOnlyAndPause();
	}
	else
	{
		SetInputModeGameOnlyAndUnPause();
	}
}
