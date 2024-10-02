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

void ATetrisPlayerControllerIngameBase::Initialize()
{
	GameMode = GetWorld()->GetAuthGameMode<ATetrisSinglePlayerGameModeBase>();

	KeyPressingFlags = EKeyFlags::None;

	InitializeCamera();
	InitializeInput();
}

void ATetrisPlayerControllerIngameBase::InitializeCamera()
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

void ATetrisPlayerControllerIngameBase::InitializeInput()
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

void ATetrisPlayerControllerIngameBase::BindInputActions(UEnhancedInputComponent* const EnhancedInputComponent)
{
	Super::BindInputActions(EnhancedInputComponent);

    // MoveLeft
    if (MoveLeftAction)
    {
        EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngameBase::OnMoveLeftStarted);
        EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Completed, this, &ATetrisPlayerControllerIngameBase::OnMoveLeftCompleted);
    }

    // MoveRight
    if (MoveRightAction)
    {
        EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngameBase::OnMoveRightStarted);
        EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &ATetrisPlayerControllerIngameBase::OnMoveRightCompleted);
    }

    // Soft Drop
    if (SoftDropAction)
    {
        EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngameBase::OnSoftDropStarted);
        EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Completed, this, &ATetrisPlayerControllerIngameBase::OnSoftDropCompleted);
    }

    // Hard Drop
    if (HardDropAction)
    {
        EnhancedInputComponent->BindAction(HardDropAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngameBase::OnHardDropStarted);
    }

    // Rotate Clockwise
    if (RotateClockwiseAction)
    {
        EnhancedInputComponent->BindAction(RotateClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngameBase::OnRotateClockwiseStarted);
    }

    // Rotate CounterClockwise
    if (RotateCounterClockwiseAction)
    {
        EnhancedInputComponent->BindAction(RotateCounterClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngameBase::OnRotateCounterClockwiseStarted);
    }

    // Hold
    if (HoldAction)
    {
        EnhancedInputComponent->BindAction(HoldAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerIngameBase::OnHoldStarted);
    }
}

void ATetrisPlayerControllerIngameBase::OnMoveLeftStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngameBase::OnMoveLeftStarted()"));
	StartTetriminoMovement(EKeyFlags::Left);
}

void ATetrisPlayerControllerIngameBase::OnMoveLeftCompleted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngameBase::OnMoveLeftCompleted()"));
	EndTetriminoMovement(EKeyFlags::Left);
}

void ATetrisPlayerControllerIngameBase::OnMoveRightStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngameBase::OnMoveRightStarted()"));
	StartTetriminoMovement(EKeyFlags::Right);
}

void ATetrisPlayerControllerIngameBase::OnMoveRightCompleted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngameBase::OnMoveRightCompleted()"));
	EndTetriminoMovement(EKeyFlags::Right);
}

void ATetrisPlayerControllerIngameBase::OnSoftDropStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngameBase::OnSoftDropStarted()"));
	if (GameMode)
	{
		EnumAddFlags(KeyPressingFlags, EKeyFlags::SoftDrop);
		GameMode->GetTetrisPlayManager()->StartSoftDrop();
	}
}

void ATetrisPlayerControllerIngameBase::OnSoftDropCompleted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngameBase::OnSoftDropCompleted()"));
	if (GameMode)
	{
		EnumRemoveFlags(KeyPressingFlags, EKeyFlags::SoftDrop);
		GameMode->GetTetrisPlayManager()->EndSoftDrop();
	}
}

void ATetrisPlayerControllerIngameBase::OnHardDropStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngameBase::OnHardDropStarted()"));
	if (GameMode)
	{
		GameMode->GetTetrisPlayManager()->DoHardDrop();
	}
}

void ATetrisPlayerControllerIngameBase::OnRotateClockwiseStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngameBase::RotateClockwise()"));
	if (GameMode)
	{
		GameMode->GetTetrisPlayManager()->DoRotation(ETetriminoRotationDirection::Clockwise);
	}
}

void ATetrisPlayerControllerIngameBase::OnRotateCounterClockwiseStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngameBase::RotateCounterClockwise()"));
	if (GameMode)
	{
		GameMode->GetTetrisPlayManager()->DoRotation(ETetriminoRotationDirection::CounterClockwise);
	}
}

void ATetrisPlayerControllerIngameBase::OnHoldStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerIngameBase::OnHoldStarted()"));
	if (GameMode)
	{
		GameMode->GetTetrisPlayManager()->HoldTetriminoInPlay();
	}
}

const FVector2D& ATetrisPlayerControllerIngameBase::GetDirectionByKeyFlag(const EKeyFlags KeyFlag)
{
	static const TMap<EKeyFlags, FVector2D> Map =
	{
		{EKeyFlags::Left, ATetriminoBase::MoveDirectionLeft},
		{EKeyFlags::Right, ATetriminoBase::MoveDirectionRight},
	};
	return Map[KeyFlag];
}

void ATetrisPlayerControllerIngameBase::StartTetriminoMovement(const EKeyFlags KeyPressed)
{
	if (GameMode)
	{
		EnumAddFlags(KeyPressingFlags, KeyPressed);

		const FVector2D& DirectionPressed = GetDirectionByKeyFlag(KeyPressed);
		GameMode->GetTetrisPlayManager()->StartAutoRepeatMovement(DirectionPressed);
	}
}

void ATetrisPlayerControllerIngameBase::EndTetriminoMovement(const EKeyFlags KeyReleased)
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
