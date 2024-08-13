// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisPlayerControllerInGameBase.h"
#include "Misc/EnumClassFlags.h"

#include "Mino.h"

#include "TetrisPlayerControllerSingle.generated.h"

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
	SoftDrop = 1 << 2,
};

ENUM_CLASS_FLAGS(EKeyFlags)

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisPlayerControllerSingle : public ATetrisPlayerControllerInGameBase
{
	GENERATED_BODY()

public:
	void Initialize();

	bool IsKeyPressed(const EKeyFlags KeyFlag) const { return EnumHasAnyFlags(KeyPressingFlags, KeyFlag); }
	bool IsSoftDropKeyPressed() const { return IsKeyPressed(EKeyFlags::SoftDrop); }

private:
	void InitializeCamera();
	void InitializeInput();

	void BindGamePlayInput();

	// 입력 콜백 함수
	void OnMoveLeftStarted(const FInputActionValue& ActionValue);
	void OnMoveLeftCompleted(const FInputActionValue& ActionValue);
	void OnMoveRightStarted(const FInputActionValue& ActionValue);
	void OnMoveRightCompleted(const FInputActionValue& ActionValue);
	void OnSoftDropStarted(const FInputActionValue& ActionValue);
	void OnSoftDropCompleted(const FInputActionValue& ActionValue);
	void OnHardDropStarted(const FInputActionValue& ActionValue);
	void OnRotateClockwiseStarted(const FInputActionValue& ActionValue);
	void OnRotateCounterClockwiseStarted(const FInputActionValue& ActionValue);
	void OnHoldStarted(const FInputActionValue& ActionValue);

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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> HoldAction;

	UPROPERTY()
	TObjectPtr<ATetrisGameModeBase> GameMode;
};