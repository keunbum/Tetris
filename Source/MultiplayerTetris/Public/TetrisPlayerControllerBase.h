// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "TetrisPlayerControllerBase.generated.h"

class UEnhancedInputComponent;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BindInputActions(UEnhancedInputComponent* const EnhancedInputComponent) { check(EnhancedInputComponent != nullptr); }
};
