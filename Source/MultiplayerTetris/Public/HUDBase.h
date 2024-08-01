// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDBase.generated.h"

class UTextBlock;
class ATetrisGameModeBase;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UHUDBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/** static methods */
	static FString GetFormattedTimeString(const float TimeInSeconds);

public:
	static constexpr float TimeDisplayUpdateInterval = 0.1f;

protected:
	UPROPERTY()
	TObjectPtr<ATetrisGameModeBase> TetrisGameMode;
};
