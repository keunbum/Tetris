// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerController.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

#include "TetriminoBase.h"
#include "TetrisSinglePlayerGameModeBase.h"
#include "TetrisPlayManager.h"

void ATetrisPlayerController::Initialize()
{
	InitializeCamera();
	InitializeInput();

	GameMode = GetWorld()->GetAuthGameMode<ATetrisSinglePlayerGameModeBase>();
	check(GameMode != nullptr);

	KeyPressingFlags = EKeyFlags::None;
}

void ATetrisPlayerController::InitializeCamera()
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

void ATetrisPlayerController::InitializeInput()
{
	BindGamePlayInput();

	if (InGameInputMappingContext)
	{
		if (UEnhancedInputLocalPlayerSubsystem* const Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InGameInputMappingContext, MappingContextDefaultPriority);
		}
	}
}

void ATetrisPlayerController::BindGamePlayInput()
{
	if (UEnhancedInputComponent* const EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// MoveLeft
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnMoveLeftStarted);
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Completed, this, &ATetrisPlayerController::OnMoveLeftCompleted);
		// MoveRight
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnMoveRightStarted);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &ATetrisPlayerController::OnMoveRightCompleted);

		// Soft Drop
		EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnSoftDropStarted);
		EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Completed, this, &ATetrisPlayerController::OnSoftDropCompleted);

		// Hard Drop
		EnhancedInputComponent->BindAction(HardDropAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnHardDropStarted);

		// RotateClockwise
		EnhancedInputComponent->BindAction(RotateClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnRotateClockwiseStarted);
		// RotateCounterClockwise
		EnhancedInputComponent->BindAction(RotateCounterClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnRotateCounterClockwiseStarted);

		// Hold
		EnhancedInputComponent->BindAction(HoldAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnHoldStarted);
	}
}

void ATetrisPlayerController::OnMoveLeftStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnMoveLeftStarted()"));
	StartTetriminoMovement(EKeyFlags::Left);
}

void ATetrisPlayerController::OnMoveLeftCompleted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnMoveLeftCompleted()"));
	EndTetriminoMovement(EKeyFlags::Left);
}

void ATetrisPlayerController::OnMoveRightStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnMoveRightStarted()"));
	StartTetriminoMovement(EKeyFlags::Right);
}

void ATetrisPlayerController::OnMoveRightCompleted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnMoveRightCompleted()"));
	EndTetriminoMovement(EKeyFlags::Right);
}

void ATetrisPlayerController::OnSoftDropStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnSoftDropStarted()"));
	EnumAddFlags(KeyPressingFlags, EKeyFlags::SoftDrop);
	GameMode->GetTetrisPlayManager()->StartSoftDrop();
}

void ATetrisPlayerController::OnSoftDropCompleted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnSoftDropCompleted()"));
	EnumRemoveFlags(KeyPressingFlags, EKeyFlags::SoftDrop);
	GameMode->GetTetrisPlayManager()->EndSoftDrop();
}

void ATetrisPlayerController::OnHardDropStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnHardDropStarted()"));
	GameMode->GetTetrisPlayManager()->DoHardDrop();
}

void ATetrisPlayerController::OnRotateClockwiseStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::RotateClockwise()"));
	GameMode->GetTetrisPlayManager()->DoRotation(ETetriminoRotationDirection::Clockwise);
}

void ATetrisPlayerController::OnRotateCounterClockwiseStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::RotateCounterClockwise()"));
	GameMode->GetTetrisPlayManager()->DoRotation(ETetriminoRotationDirection::CounterClockwise);
}

void ATetrisPlayerController::OnHoldStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnHoldStarted()"));
	GameMode->GetTetrisPlayManager()->HoldTetriminoInPlay();
}

const FVector2D& ATetrisPlayerController::GetDirectionByKeyFlag(const EKeyFlags KeyFlag)
{
	static const TMap<EKeyFlags, FVector2D> Map =
	{
		{EKeyFlags::Left, ATetriminoBase::MoveDirectionLeft},
		{EKeyFlags::Right, ATetriminoBase::MoveDirectionRight},
	};
	return Map[KeyFlag];
}

void ATetrisPlayerController::StartTetriminoMovement(const EKeyFlags KeyPressed)
{
	EnumAddFlags(KeyPressingFlags, KeyPressed);

	const FVector2D& DirectionPressed = GetDirectionByKeyFlag(KeyPressed);
	GameMode->GetTetrisPlayManager()->StartMovement(DirectionPressed);
}

void ATetrisPlayerController::EndTetriminoMovement(const EKeyFlags KeyReleased)
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
