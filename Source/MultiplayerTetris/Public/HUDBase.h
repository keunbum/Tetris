// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDBase.generated.h"

class UTextBlock;
class ATetrisPlayerStateBase;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UHUDBase : public UUserWidget
{
	GENERATED_BODY()

public:
	FString GetElapsedTimeFormatted(const float ElapsedTime) const { return UHUDBase::GetFormattedTime(ElapsedTime); }

	/** static methods */
	static FString GetFormattedTime(const float TimeInSeconds);
};
