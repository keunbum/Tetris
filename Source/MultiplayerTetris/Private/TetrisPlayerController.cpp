// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "TetrisPlayerController.h"

#include "Camera/CameraActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

#include "TetrominoPawn.h"

void ATetrisPlayerController::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void ATetrisPlayerController::SetCamera()
{
	if (UWorld* const World = GetWorld())
	{
		if (AActor* const CameraActor = UGameplayStatics::GetActorOfClass(World, ACameraActor::StaticClass()))
		{
			// ViewTarget을 GameStartCamera로 설정합니다.
			SetViewTarget(CameraActor);
		}
	}
}

void ATetrisPlayerController::SetInput()
{
	if (UEnhancedInputComponent* const EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ATetrisPlayerController::MoveLeft);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ATetrisPlayerController::MoveRight);
		EnhancedInputComponent->BindAction(SoftDropAction, ETriggerEvent::Started, this, &ATetrisPlayerController::SoftDrop);
		EnhancedInputComponent->BindAction(HardDropAction, ETriggerEvent::Started, this, &ATetrisPlayerController::HardDrop);
	}

	if (UEnhancedInputLocalPlayerSubsystem* const Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (TetrisInputMappingContext)
		{
			const int32 Priority = 0;
			Subsystem->AddMappingContext(TetrisInputMappingContext, Priority);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TetrisInputMappingContext is null"));
		}
	}
}

void ATetrisPlayerController::Initialize()
{
	SetCamera();
	SetInput();
	TetrominoPawn = GetPawn<ATetrominoPawn>();
	ensureMsgf(TetrominoPawn != nullptr, TEXT("FUCK. GetPawn<ATetrominoPawn>() failed."));
}

void ATetrisPlayerController::MoveLeft(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrisPlayerController::MoveLeft()!"));
	TetrominoPawn->MoveLeft();
}

void ATetrisPlayerController::MoveRight(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrisPlayerController::MoveRight()!"));
	TetrominoPawn->MoveRight();
}

void ATetrisPlayerController::SoftDrop(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrisPlayerController::SoftDrop()!"));
	TetrominoPawn->SoftDrop();
}

void ATetrisPlayerController::HardDrop(const FInputActionValue& ActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Hi! I'm ATetrisPlayerController::HardDrop()!"));
	TetrominoPawn->HardDrop();
}
