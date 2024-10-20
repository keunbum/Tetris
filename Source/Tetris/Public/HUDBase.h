// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDBase.generated.h"

class UTextBlockBase;

/**
 * 
 */
UCLASS(Abstract)
class TETRIS_API UHUDBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/** static methods */
	static FString GetFormattedTimeString(const float TimeInSeconds);
	static void UpdateTextBlockWithNameAndValue(UTextBlockBase* const TextBlock, const FString& Name, const FString& Value);
	static void UpdateTextBlock(UTextBlockBase* const TextBlock, const FString& NewText);
};
