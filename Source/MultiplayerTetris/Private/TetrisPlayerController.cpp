// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "TetrisPlayerController.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

#include "Tetrimino.h"
#include "TetrominoPawn.h"

void ATetrisPlayerController::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void ATetrisPlayerController::Initialize()
{
	InitializeCamera();
	InitializeInput();

	TetrominoPawn = GetPawn<ATetrominoPawn>();
	check(TetrominoPawn != nullptr);
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
		EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnMoveSoftDropStarted);
		EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Completed, this, &ATetrisPlayerController::OnMoveSoftDropCompleted);

		// Hard Drop
		EnhancedInputComponent->BindAction(HardDropAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnMoveHardDropStarted);

		// RotateClockwise
		EnhancedInputComponent->BindAction(RotateClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnMoveHardRotateClockwiseStarted);
		// RotateCounterClockwise
		EnhancedInputComponent->BindAction(RotateCounterClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnMoveHardCounterClockwiseStarted);
	}
}

void ATetrisPlayerController::OnMoveLeftStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnMoveLeftStarted()"));
	StartPawnMovement(EKeyFlags::Left);
}

void ATetrisPlayerController::OnMoveLeftCompleted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnMoveLeftCompleted()"));
	EndPawnMovement(EKeyFlags::Left);
}

void ATetrisPlayerController::OnMoveRightStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnMoveRightStarted()"));
	StartPawnMovement(EKeyFlags::Right);
}

void ATetrisPlayerController::OnMoveRightCompleted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnMoveRightCompleted()"));
	EndPawnMovement(EKeyFlags::Right);
}

void ATetrisPlayerController::OnMoveSoftDropStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnMoveSoftDropStarted()"));
	TetrominoPawn->StartSoftDrop();
}

void ATetrisPlayerController::OnMoveSoftDropCompleted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnMoveSoftDropCompleted()"));
	TetrominoPawn->EndSoftDrop();
}

void ATetrisPlayerController::OnMoveHardDropStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::OnMoveHardDropStarted()"));
	TetrominoPawn->StartHardDrop();
}

void ATetrisPlayerController::OnMoveHardRotateClockwiseStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::RotateClockwise()"));
	TetrominoPawn->StartRotate(+1);
}

void ATetrisPlayerController::OnMoveHardCounterClockwiseStarted(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::RotateCounterClockwise()"));
	TetrominoPawn->StartRotate(-1);
}

const FVector2D& ATetrisPlayerController::GetDirectionByKeyFlag(const EKeyFlags KeyFlag)
{
	static const TMap<EKeyFlags, FVector2D> Map =
	{
		{EKeyFlags::Left, ATetrimino::MoveDirectionLeft},
		{EKeyFlags::Right, ATetrimino::MoveDirectionRight},
	};
	return Map[KeyFlag];
}

void ATetrisPlayerController::StartPawnMovement(const EKeyFlags KeyPressed)
{
	EnumAddFlags(KeyPressingFlags, KeyPressed);

	const FVector2D& DirectionPressed = GetDirectionByKeyFlag(KeyPressed);
	TetrominoPawn->StartMovement(DirectionPressed);
}

void ATetrisPlayerController::EndPawnMovement(const EKeyFlags KeyReleased)
{
	const bool bIsPressingLeftRightBoth = EnumHasAllFlags(KeyPressingFlags, (EKeyFlags::Left | EKeyFlags::Right));
	if (bIsPressingLeftRightBoth)
	{
		const FVector2D& DirectionReleased = GetDirectionByKeyFlag(KeyReleased);
		// 테트로미노 현재 이동 중인 방향 키를 뗐을 경우
		if (TetrominoPawn->GetMovementDirection() == DirectionReleased)
		{
			const FVector2D OppositeDirection = -DirectionReleased;
			TetrominoPawn->StartMovement(OppositeDirection);
		}
	}
	else
	{
		TetrominoPawn->EndMovement();
	}
	EnumRemoveFlags(KeyPressingFlags, KeyReleased);
}
