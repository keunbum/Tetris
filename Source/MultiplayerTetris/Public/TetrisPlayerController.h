// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"

#include "TetrisPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ATetrominoPawn;

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
	void SetCamera();
	void SetInput();
	void Initialize();

	// 입력 콜백 함수
	void MoveLeft(const FInputActionValue& ActionValue);
	void MoveRight(const FInputActionValue& ActionValue);
	void SoftDrop(const FInputActionValue& ActionValue);
	void StopSoftDrop(const FInputActionValue& ActionValue);
	void HardDrop(const FInputActionValue& ActionValue);

private:
	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> TetrisInputMappingContext;

	// Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveLeftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveRightAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> SoftDropAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> HardDropAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetrominoPawn> TetrominoPawn;
};
