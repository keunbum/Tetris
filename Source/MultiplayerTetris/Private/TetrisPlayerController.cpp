// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "TetrisPlayerController.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

#include "Tetromino.h"
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
	if (UEnhancedInputComponent* const EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// MoveLeft
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ATetrisPlayerController::MoveLeft);
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Completed, this, &ATetrisPlayerController::EndMoveLeft);
		// MoveRight
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ATetrisPlayerController::MoveRight);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &ATetrisPlayerController::EndMoveRight);

		// Soft Drop
		EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Started, this, &ATetrisPlayerController::SoftDrop);
		EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Completed, this, &ATetrisPlayerController::EndSoftDrop);

		// Hard Drop
		EnhancedInputComponent->BindAction(HardDropAction, ETriggerEvent::Started, this, &ATetrisPlayerController::HardDrop);

		// RotateClockwise
		EnhancedInputComponent->BindAction(RotateClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerController::RotateClockwise);
		// RotateCounterClockwise
		EnhancedInputComponent->BindAction(RotateCounterClockwiseAction, ETriggerEvent::Started, this, &ATetrisPlayerController::RotateCounterClockwise);
	}

	if (TetrisInputMappingContext)
	{
		if (UEnhancedInputLocalPlayerSubsystem* const Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			static constexpr int32 Priority = 0;
			Subsystem->AddMappingContext(TetrisInputMappingContext, Priority);
		}
	}
}

void ATetrisPlayerController::MoveLeft(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::MoveLeft()"));
	MoveTo(EKeyFlags::Left);
}

void ATetrisPlayerController::MoveRight(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::MoveRight()"));
	MoveTo(EKeyFlags::Right);
}

void ATetrisPlayerController::EndMoveLeft(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::EndMoveLeft()"));
	EndMovement(EKeyFlags::Left);
}

void ATetrisPlayerController::EndMoveRight(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::EndMoveRight()"));
	EndMovement(EKeyFlags::Right);
}

void ATetrisPlayerController::SoftDrop(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::SoftDrop()"));
	TetrominoPawn->OnSoftDrop();
}

void ATetrisPlayerController::EndSoftDrop(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::EndSoftDrop()"));
	TetrominoPawn->OnEndSoftDrop();
}

void ATetrisPlayerController::HardDrop(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::HardDrop()"));
	TetrominoPawn->OnHardDrop();
}

void ATetrisPlayerController::RotateClockwise(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::RotateClockwise()"));
	TetrominoPawn->OnRotateTo(+1);
}

void ATetrisPlayerController::RotateCounterClockwise(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ATetrisPlayerController::RotateCounterClockwise()"));
	TetrominoPawn->OnRotateTo(-1);
}

const FVector2D& ATetrisPlayerController::GetDirectionByKeyFlag(const EKeyFlags KeyFlag)
{
	static const TMap<EKeyFlags, FVector2D> Map =
	{
		{EKeyFlags::Left, ATetromino::MoveDirectionLeft},
		{EKeyFlags::Right, ATetromino::MoveDirectionRight},
	};
	return Map[KeyFlag];
}

void ATetrisPlayerController::MoveTo(const EKeyFlags KeyPressed)
{
	EnumAddFlags(KeyPressingFlags, KeyPressed);

	const FVector2D& DirectionPressed = GetDirectionByKeyFlag(KeyPressed);
	TetrominoPawn->OnMove(DirectionPressed);
}

void ATetrisPlayerController::EndMovement(const EKeyFlags KeyReleased)
{
	EnumRemoveFlags(KeyPressingFlags, KeyReleased);

	const bool bIsPressingLeftRightBoth = EnumHasAnyFlags(KeyPressingFlags, (EKeyFlags::Left | EKeyFlags::Right));
	if (bIsPressingLeftRightBoth)
	{
		const FVector2D& DirectionReleased = GetDirectionByKeyFlag(KeyReleased);
		// 테트로미노 현재 이동 방향과 뗀 키 방향이 같은 경우
		if (TetrominoPawn->GetMovementDirection() == DirectionReleased)
		{
			const FVector2D OppositeDirection = -DirectionReleased;
			TetrominoPawn->OnMove(OppositeDirection);
		}
	}
	else
	{
		TetrominoPawn->OnEndMove();
	}
}
