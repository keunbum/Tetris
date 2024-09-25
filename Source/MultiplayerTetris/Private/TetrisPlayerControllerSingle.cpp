// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerControllerSingle.h"

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Kismet/GameplayStatics.h"
#include "PauseMenuWidget.h"
#include "GameOverMenuWidget.h"

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

	PauseMenuWidget->AddToViewport();

	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ATetrisPlayerControllerSingle::SetInputModeUIOnlyAndGameOver()
{
	if (!GameOverMenuWidget)
	{
		check(GameOverMenuWidgetClass != nullptr);
		GameOverMenuWidget = CreateWidget<UGameOverMenuWidget>(this, GameOverMenuWidgetClass);
		check(GameOverMenuWidget != nullptr);
	}

	// Set the input mode to UIOnly
	const FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	GameOverMenuWidget->AddToViewport();

	// 이거는.. 추후에 수정될 여지 있음.
	// 일부 로직만 멈추고, 배경이나 효과 같은 건 그대로 돌아가야 그럴싸하기 때문.
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ATetrisPlayerControllerSingle::BindInputActions(UEnhancedInputComponent* const EnhancedInputComponent)
{
	Super::BindInputActions(EnhancedInputComponent);

	// Pause
	if (PauseAction)
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerSingle::OnPause);
	}
}

void ATetrisPlayerControllerSingle::OnPause(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::OnPause()"));
	SetInputModeUIOnlyAndPause();
}
