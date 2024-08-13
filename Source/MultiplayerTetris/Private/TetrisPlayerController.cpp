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

void ATetrisPlayerControllerSingle::Initialize()
{
	InitializeCamera();
	InitializeInput();

	GameMode = GetWorld()->GetAuthGameMode<ATetrisSinglePlayerGameModeBase>();
	check(GameMode != nullptr);

	KeyPressingFlags = EKeyFlags::None;
}

void ATetrisPlayerControllerSingle::InitializeCamera()
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

void ATetrisPlayerControllerSingle::InitializeInput()
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

void ATetrisPlayerControllerSingle::BindGamePlayInput()
{
	if (UEnhancedInputComponent* const EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// MoveLeft
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerSingle::OnMoveLeftStarted);
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Completed, this, &ATetrisPlayerControllerSingle::OnMoveLeftCompleted);
		// MoveRight
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerSingle::OnMoveRightStarted);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &ATetrisPlayerControllerSingle::OnMoveRightCompleted);

		// Soft Drop
		EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerSingle::OnSoftDropStarted);
		EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Completed, this, &ATetrisPlayerControllerSingle::OnSoftDropCompleted);

		// Hard Drop
		EnhancedInputComponent->BindAction(HardDropAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerSingle::OnHardDropStarted);

		// RotateClockwise
		EnhancedInputComponent->BindAction(RotateClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerSingle::OnRotateClockwiseStarted);
		// RotateCounterClockwise
		EnhancedInputComponent->BindAction(RotateCounterClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerSingle::OnRotateCounterClockwiseStarted);

		// Hold
		EnhancedInputComponent->BindAction(HoldAction, ETriggerEvent::Started, this, &ATetrisPlayerControllerSingle::OnHoldStarted);
	}
}

void ATetrisPlayerControllerSingle::OnMoveLeftStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::OnMoveLeftStarted()"));
	StartTetriminoMovement(EKeyFlags::Left);
}

void ATetrisPlayerControllerSingle::OnMoveLeftCompleted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::OnMoveLeftCompleted()"));
	EndTetriminoMovement(EKeyFlags::Left);
}

void ATetrisPlayerControllerSingle::OnMoveRightStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::OnMoveRightStarted()"));
	StartTetriminoMovement(EKeyFlags::Right);
}

void ATetrisPlayerControllerSingle::OnMoveRightCompleted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::OnMoveRightCompleted()"));
	EndTetriminoMovement(EKeyFlags::Right);
}

void ATetrisPlayerControllerSingle::OnSoftDropStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::OnSoftDropStarted()"));
	EnumAddFlags(KeyPressingFlags, EKeyFlags::SoftDrop);
	GameMode->GetTetrisPlayManager()->StartSoftDrop();
}

void ATetrisPlayerControllerSingle::OnSoftDropCompleted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::OnSoftDropCompleted()"));
	EnumRemoveFlags(KeyPressingFlags, EKeyFlags::SoftDrop);
	GameMode->GetTetrisPlayManager()->EndSoftDrop();
}

void ATetrisPlayerControllerSingle::OnHardDropStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::OnHardDropStarted()"));
	GameMode->GetTetrisPlayManager()->DoHardDrop();
}

void ATetrisPlayerControllerSingle::OnRotateClockwiseStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::RotateClockwise()"));
	GameMode->GetTetrisPlayManager()->DoRotation(ETetriminoRotationDirection::Clockwise);
}

void ATetrisPlayerControllerSingle::OnRotateCounterClockwiseStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::RotateCounterClockwise()"));
	GameMode->GetTetrisPlayManager()->DoRotation(ETetriminoRotationDirection::CounterClockwise);
}

void ATetrisPlayerControllerSingle::OnHoldStarted(const FInputActionValue& ActionValue)
{
	//UE_LOG(LogTemp, Display, TEXT("ATetrisPlayerControllerSingle::OnHoldStarted()"));
	GameMode->GetTetrisPlayManager()->HoldTetriminoInPlay();
}

const FVector2D& ATetrisPlayerControllerSingle::GetDirectionByKeyFlag(const EKeyFlags KeyFlag)
{
	static const TMap<EKeyFlags, FVector2D> Map =
	{
		{EKeyFlags::Left, ATetriminoBase::MoveDirectionLeft},
		{EKeyFlags::Right, ATetriminoBase::MoveDirectionRight},
	};
	return Map[KeyFlag];
}

void ATetrisPlayerControllerSingle::StartTetriminoMovement(const EKeyFlags KeyPressed)
{
	EnumAddFlags(KeyPressingFlags, KeyPressed);

	const FVector2D& DirectionPressed = GetDirectionByKeyFlag(KeyPressed);
	GameMode->GetTetrisPlayManager()->StartMovement(DirectionPressed);
}

void ATetrisPlayerControllerSingle::EndTetriminoMovement(const EKeyFlags KeyReleased)
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
