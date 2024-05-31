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
class ATetrominoPawn;

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

public:
	void SetInputModeUI();
	void SetInputModeGame();
	
protected:
	virtual void BeginPlay() override;

private:
	void Initialize();
	void InitializeCamera();
	void InitializeInput();

	// 입력 콜백 함수
	void MoveLeft(const FInputActionValue& ActionValue);
	void MoveRight(const FInputActionValue& ActionValue);
	void EndMoveLeft(const FInputActionValue& ActionValue);
	void EndMoveRight(const FInputActionValue& ActionValue);
	void SoftDrop(const FInputActionValue& ActionValue);
	void EndSoftDrop(const FInputActionValue& ActionValue);
	void HardDrop(const FInputActionValue& ActionValue);
	void RotateClockwise(const FInputActionValue& ActionValue);
	void RotateCounterClockwise(const FInputActionValue& ActionValue);

	static const FVector2D& GetDirectionByKeyFlag(const EKeyFlags KeyFlag);

	void MoveTo(const EKeyFlags KeyPressed);
	void EndMovement(const EKeyFlags KeyReleased);

private:
	static constexpr int32 MappingContextDefaultPriority = 0;

	UPROPERTY(EditDefaultsOnly)
	float OrthoWidth = 8000 * AMino::MinoScale;

	// 입력 상태 추적
	UPROPERTY()
	EKeyFlags KeyPressingFlags;


	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> GamePlayInputMappingContext;


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


	// Player Pawn
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetrominoPawn> TetrominoPawn;
};
