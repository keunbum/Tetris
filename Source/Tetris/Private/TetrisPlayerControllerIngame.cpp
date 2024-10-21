// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerControllerIngame.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"

#include "TetriminoBase.h"
#include "TetrisGameModeIngame.h"
#include "TetrisPlayManager.h"

void ATetrisPlayerControllerIngame::Initialize()
{
	GameMode = GetWorld()->GetAuthGameMode<ATetrisGameModeIngame>();

	KeyPressingFlags = EKeyFlags::None;

	InitializeInput();
}

void ATetrisPlayerControllerIngame::InitializeInput()
{
	UEnhancedInputComponent* const EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!ensureMsgf(EnhancedInputComponent, TEXT("InputComponent is not UEnhancedInputComponent.")))
	{
		return;
	}
	
	BindInputActions(EnhancedInputComponent);

	if (InGameInputMappingContext)
	{
		if (UEnhancedInputLocalPlayerSubsystem* const Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InGameInputMappingContext, MappingContextDefaultPriority);
		}
	}

	// Set focus to the game viewport
	FSlateApplication::Get().SetAllUserFocusToGameViewport();
}

void ATetrisPlayerControllerIngame::BindInputActions(UEnhancedInputComponent* const EnhancedInputComponent)
{
	Super::BindInputActions(EnhancedInputComponent);

    // MoveLeft
    if (MoveLeftAction)
    {
        EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngame::OnMoveLeftStarted);
        EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Completed, this, &ATetrisPlayerControllerIngame::OnMoveLeftCompleted);
    }

    // MoveRight
    if (MoveRightAction)
    {
        EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngame::OnMoveRightStarted);
        EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &ATetrisPlayerControllerIngame::OnMoveRightCompleted);
    }

    // Soft Drop
    if (SoftDropAction)
    {
        EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngame::OnSoftDropStarted);
        EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Completed, this, &ATetrisPlayerControllerIngame::OnSoftDropCompleted);
    }

    // Hard Drop
    if (HardDropAction)
    {
        EnhancedInputComponent->BindAction(HardDropAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngame::OnHardDropStarted);
    }

    // Rotate Clockwise
    if (RotateClockwiseAction)
    {
        EnhancedInputComponent->BindAction(RotateClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngame::OnRotateClockwiseStarted);
    }

    // Rotate CounterClockwise
    if (RotateCounterClockwiseAction)
    {
        EnhancedInputComponent->BindAction(RotateCounterClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngame::OnRotateCounterClockwiseStarted);
    }

    // Hold
    if (HoldAction)
    {
        EnhancedInputComponent->BindAction(HoldAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngame::OnHoldStarted);
    }
}

void ATetrisPlayerControllerIngame::OnMoveLeftStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngame::OnMoveLeftStarted()"));
	StartTetriminoMovement(EKeyFlags::Left);
}

void ATetrisPlayerControllerIngame::OnMoveLeftCompleted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngame::OnMoveLeftCompleted()"));
	EndTetriminoMovement(EKeyFlags::Left);
}

void ATetrisPlayerControllerIngame::OnMoveRightStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngame::OnMoveRightStarted()"));
	StartTetriminoMovement(EKeyFlags::Right);
}

void ATetrisPlayerControllerIngame::OnMoveRightCompleted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngame::OnMoveRightCompleted()"));
	EndTetriminoMovement(EKeyFlags::Right);
}

void ATetrisPlayerControllerIngame::OnSoftDropStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngame::OnSoftDropStarted()"));
	if (GameMode)
	{
		EnumAddFlags(KeyPressingFlags, EKeyFlags::SoftDrop);
		GameMode->GetTetrisPlayManager()->StartSoftDrop();
	}
}

void ATetrisPlayerControllerIngame::OnSoftDropCompleted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngame::OnSoftDropCompleted()"));
	if (GameMode)
	{
		EnumRemoveFlags(KeyPressingFlags, EKeyFlags::SoftDrop);
		GameMode->GetTetrisPlayManager()->EndSoftDrop();
	}
}

void ATetrisPlayerControllerIngame::OnHardDropStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngame::OnHardDropStarted()"));
	if (GameMode)
	{
		GameMode->GetTetrisPlayManager()->DoHardDrop();
	}
}

void ATetrisPlayerControllerIngame::OnRotateClockwiseStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngame::RotateClockwise()"));
	if (GameMode)
	{
		GameMode->GetTetrisPlayManager()->DoRotation(ETetriminoRotationDirection::Clockwise);
	}
}

void ATetrisPlayerControllerIngame::OnRotateCounterClockwiseStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngame::RotateCounterClockwise()"));
	if (GameMode)
	{
		GameMode->GetTetrisPlayManager()->DoRotation(ETetriminoRotationDirection::CounterClockwise);
	}
}

void ATetrisPlayerControllerIngame::OnHoldStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngame::OnHoldStarted()"));
	if (GameMode)
	{
		GameMode->GetTetrisPlayManager()->HoldTetriminoInPlay();
	}
}

const FVector2D& ATetrisPlayerControllerIngame::GetDirectionByKeyFlag(const EKeyFlags KeyFlag)
{
	static const TMap<EKeyFlags, FVector2D> Map =
	{
		{EKeyFlags::Left, ATetriminoBase::MoveDirectionLeft},
		{EKeyFlags::Right, ATetriminoBase::MoveDirectionRight},
	};
	return Map[KeyFlag];
}

void ATetrisPlayerControllerIngame::StartTetriminoMovement(const EKeyFlags KeyPressed)
{
	if (GameMode)
	{
		EnumAddFlags(KeyPressingFlags, KeyPressed);

		const FVector2D& DirectionPressed = GetDirectionByKeyFlag(KeyPressed);
		GameMode->GetTetrisPlayManager()->StartAutoRepeatMovement(DirectionPressed);
	}
}

void ATetrisPlayerControllerIngame::EndTetriminoMovement(const EKeyFlags KeyReleased)
{
	if (GameMode)
	{
		const bool bIsPressingLeftRightBoth = EnumHasAllFlags(KeyPressingFlags, (EKeyFlags::Left | EKeyFlags::Right));
		if (bIsPressingLeftRightBoth)
		{
			const FVector2D& DirectionReleased = GetDirectionByKeyFlag(KeyReleased);
			// 테트로미노 현재 이동 중인 방향 키를 뗐을 경우
			if (GameMode->GetTetrisPlayManager()->GetTetriminoMovementDirection() == DirectionReleased)
			{
				const FVector2D OppositeDirection = -DirectionReleased;
				GameMode->GetTetrisPlayManager()->StartAutoRepeatMovement(OppositeDirection);
			}
		}
		else
		{
			GameMode->GetTetrisPlayManager()->EndAutoRepeatMovement();
		}
		EnumRemoveFlags(KeyPressingFlags, KeyReleased);
	}
}
