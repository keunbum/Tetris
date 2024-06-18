// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Misc/EnumClassFlags.h"

#include "Mino.h"

#include "TetrisPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ATetrisGameModeBase;
class ATetrisPlayManager;

UENUM()
enum class EKeyFlags : uint8
{
	None = 0,
	Left = 1 << 0,
	Right = 1 << 1,
};

ENUM_CLASS_FLAGS(EKeyFlags)

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void Initialize();
	void InitializeCamera();
	void InitializeInput();

	void BindGamePlayInput();

	// 입력 콜백 함수
	void OnMoveLeftStarted(const FInputActionValue& ActionValue);
	void OnMoveLeftCompleted(const FInputActionValue& ActionValue);
	void OnMoveRightStarted(const FInputActionValue& ActionValue);
	void OnMoveRightCompleted(const FInputActionValue& ActionValue);
	void OnMoveSoftDropStarted(const FInputActionValue& ActionValue);
	void OnMoveSoftDropCompleted(const FInputActionValue& ActionValue);
	void OnMoveHardDropStarted(const FInputActionValue& ActionValue);
	void OnMoveHardRotateClockwiseStarted(const FInputActionValue& ActionValue);
	void OnMoveHardCounterClockwiseStarted(const FInputActionValue& ActionValue);

	static const FVector2D& GetDirectionByKeyFlag(const EKeyFlags KeyFlag);

	void StartTetriminoMovement(const EKeyFlags KeyPressed);
	void EndTetriminoMovement(const EKeyFlags KeyReleased);

private:
	static constexpr int32 MappingContextDefaultPriority = 0;

	UPROPERTY(EditDefaultsOnly)
	float OrthoWidth = 8000 * UMino::MinoScale;

	// 입력 상태 추적
	UPROPERTY()
	EKeyFlags KeyPressingFlags;


	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InGameInputMappingContext;


	// Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveLeftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveRightAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> SoftDropAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> HardDropAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> RotateClockwiseAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> RotateCounterClockwiseAction;

	UPROPERTY()
	TObjectPtr<ATetrisGameModeBase> GameMode;
};
