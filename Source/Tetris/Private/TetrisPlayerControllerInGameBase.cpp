// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerControllerInGameBase.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"
#include "Kismet/GameplayStatics.h"

#include "TetriminoBase.h"
#include "TetrisSinglePlayerGameModeBase.h"
#include "TetrisPlayManager.h"

void ATetrisPlayerControllerInGameBase::Initialize()
{
	GameMode = GetWorld()->GetAuthGameMode<ATetrisSinglePlayerGameModeBase>();

	KeyPressingFlags = EKeyFlags::None;

	InitializeCamera();
	InitializeInput();
}

void ATetrisPlayerControllerInGameBase::InitializeCamera()
{
	if (UWorld* const World = GetWorld())
	{
		if (ACameraActor* const CameraActor = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(World, ACameraActor::StaticClass())))
		{
			// ViewTarget을 GameStartCamera로 설정.
			SetViewTarget(CameraActor);

			// 카메라를 정사영 모드로 설정.
			if (CameraActor->GetCameraComponent())
			{
				CameraActor->GetCameraComponent()->SetProjectionMode(ECameraProjectionMode::Orthographic);
				CameraActor->GetCameraComponent()->OrthoWidth = OrthoWidth; // 필요에 따라 조정
			}
		}
	}
}

void ATetrisPlayerControllerInGameBase::InitializeInput()
{
	UEnhancedInputComponent* const EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
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

void ATetrisPlayerControllerInGameBase::BindInputActions(UEnhancedInputComponent* const EnhancedInputComponent)
{
	Super::BindInputActions(EnhancedInputComponent);

    // MoveLeft
    if (MoveLeftAction)
    {
        EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerInGameBase::OnMoveLeftStarted);
        EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Completed, this, &ATetrisPlayerControllerInGameBase::OnMoveLeftCompleted);
    }

    // MoveRight
    if (MoveRightAction)
    {
        EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerInGameBase::OnMoveRightStarted);
        EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &ATetrisPlayerControllerInGameBase::OnMoveRightCompleted);
    }

    // Soft Drop
    if (SoftDropAction)
    {
        EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerInGameBase::OnSoftDropStarted);
        EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Completed, this, &ATetrisPlayerControllerInGameBase::OnSoftDropCompleted);
    }

    // Hard Drop
    if (HardDropAction)
    {
        EnhancedInputComponent->BindAction(HardDropAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerInGameBase::OnHardDropStarted);
    }

    // Rotate Clockwise
    if (RotateClockwiseAction)
    {
        EnhancedInputComponent->BindAction(RotateClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerInGameBase::OnRotateClockwiseStarted);
    }

    // Rotate CounterClockwise
    if (RotateCounterClockwiseAction)
    {
        EnhancedInputComponent->BindAction(RotateCounterClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerInGameBase::OnRotateCounterClockwiseStarted);
    }

    // Hold
    if (HoldAction)
    {
        EnhancedInputComponent->BindAction(HoldAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerInGameBase::OnHoldStarted);
    }
}

void ATetrisPlayerControllerInGameBase::OnMoveLeftStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerInGameBase::OnMoveLeftStarted()"));
	StartTetriminoMovement(EKeyFlags::Left);
}

void ATetrisPlayerControllerInGameBase::OnMoveLeftCompleted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerInGameBase::OnMoveLeftCompleted()"));
	EndTetriminoMovement(EKeyFlags::Left);
}

void ATetrisPlayerControllerInGameBase::OnMoveRightStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerInGameBase::OnMoveRightStarted()"));
	StartTetriminoMovement(EKeyFlags::Right);
}

void ATetrisPlayerControllerInGameBase::OnMoveRightCompleted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerInGameBase::OnMoveRightCompleted()"));
	EndTetriminoMovement(EKeyFlags::Right);
}

void ATetrisPlayerControllerInGameBase::OnSoftDropStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerInGameBase::OnSoftDropStarted()"));
	if (GameMode)
	{
		EnumAddFlags(KeyPressingFlags, EKeyFlags::SoftDrop);
		GameMode->GetTetrisPlayManager()->StartSoftDrop();
	}
}

void ATetrisPlayerControllerInGameBase::OnSoftDropCompleted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerInGameBase::OnSoftDropCompleted()"));
	if (GameMode)
	{
		EnumRemoveFlags(KeyPressingFlags, EKeyFlags::SoftDrop);
		GameMode->GetTetrisPlayManager()->EndSoftDrop();
	}
}

void ATetrisPlayerControllerInGameBase::OnHardDropStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerInGameBase::OnHardDropStarted()"));
	if (GameMode)
	{
		GameMode->GetTetrisPlayManager()->DoHardDrop();
	}
}

void ATetrisPlayerControllerInGameBase::OnRotateClockwiseStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerInGameBase::RotateClockwise()"));
	if (GameMode)
	{
		GameMode->GetTetrisPlayManager()->DoRotation(ETetriminoRotationDirection::Clockwise);
	}
}

void ATetrisPlayerControllerInGameBase::OnRotateCounterClockwiseStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerInGameBase::RotateCounterClockwise()"));
	if (GameMode)
	{
		GameMode->GetTetrisPlayManager()->DoRotation(ETetriminoRotationDirection::CounterClockwise);
	}
}

void ATetrisPlayerControllerInGameBase::OnHoldStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerInGameBase::OnHoldStarted()"));
	if (GameMode)
	{
		GameMode->GetTetrisPlayManager()->HoldTetriminoInPlay();
	}
}

const FVector2D& ATetrisPlayerControllerInGameBase::GetDirectionByKeyFlag(const EKeyFlags KeyFlag)
{
	static const TMap<EKeyFlags, FVector2D> Map =
	{
		{EKeyFlags::Left, ATetriminoBase::MoveDirectionLeft},
		{EKeyFlags::Right, ATetriminoBase::MoveDirectionRight},
	};
	return Map[KeyFlag];
}

void ATetrisPlayerControllerInGameBase::StartTetriminoMovement(const EKeyFlags KeyPressed)
{
	if (GameMode)
	{
		EnumAddFlags(KeyPressingFlags, KeyPressed);

		const FVector2D& DirectionPressed = GetDirectionByKeyFlag(KeyPressed);
		GameMode->GetTetrisPlayManager()->StartMovement(DirectionPressed);
	}
}

void ATetrisPlayerControllerInGameBase::EndTetriminoMovement(const EKeyFlags KeyReleased)
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
				GameMode->GetTetrisPlayManager()->StartMovement(OppositeDirection);
			}
		}
		else
		{
			GameMode->GetTetrisPlayManager()->EndMovement();
		}
		EnumRemoveFlags(KeyPressingFlags, KeyReleased);
	}
}
