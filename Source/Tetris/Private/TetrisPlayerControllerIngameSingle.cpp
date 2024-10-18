// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerControllerIngameSingle.h"

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "Kismet/GameplayStatics.h"
#include "TetrisWidgetMenuPause.h"
#include "TetrisWidgetMenuGameover.h"

void ATetrisPlayerControllerIngameSingle::SetInputModeGameOnlyAndUnPause()
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

void ATetrisPlayerControllerIngameSingle::SetInputModeUIOnlyAndPause()
{
	if (!PauseMenuWidget)
	{
		PauseMenuWidget = CreateWidget<UTetrisWidgetMenuPause>(this, PauseMenuWidgetClass);
	}

	// Set the input mode to UIOnly
	const FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	if (PauseMenuWidget)
	{
		PauseMenuWidget->AddToViewport();
	}

	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ATetrisPlayerControllerIngameSingle::SetInputModeUIOnlyAndGameOver()
{
	if (!GameOverMenuWidget)
	{
		GameOverMenuWidget = CreateWidget<UTetrisWidgetMenuGameover>(this, GameOverMenuWidgetClass);
	}

	// Set the input mode to UIOnly
	const FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	if (GameOverMenuWidget)
	{
		GameOverMenuWidget->AddToViewport();
	}

	// 이거는.. 추후에 수정될 여지 있음.
	// 일부 로직만 멈추고, 배경이나 효과 같은 건 그대로 돌아가야 그럴싸하기 때문.
	// 이렇게 해야 Pause 같은 입력이 먹히질 않기 때문에 일단 편의상 이렇게 구현.
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ATetrisPlayerControllerIngameSingle::BindInputActions(UEnhancedInputComponent* const EnhancedInputComponent)
{
	Super::BindInputActions(EnhancedInputComponent);

	// Pause
	if (PauseAction)
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngameSingle::OnPause);
	}
}

void ATetrisPlayerControllerIngameSingle::OnPause(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngameSingle::OnPause()"));
	SetInputModeUIOnlyAndPause();
}
