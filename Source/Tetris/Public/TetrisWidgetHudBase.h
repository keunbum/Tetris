// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisWidgetBase.h"
#include "TetrisWidgetHudBase.generated.h"

class UTextBlockBase;

/**
 * 
 */
UCLASS(Abstract)
class TETRIS_API UTetrisWidgetHudBase : public UTetrisWidgetBase
{
	GENERATED_BODY()

public:
	/** static methods */
	static FString GetFormattedTimeString(const float TimeInSeconds);
	static void UpdateTextBlockWithNameAndValue(UTextBlockBase* const TextBlock, const FString& Name, const FString& Value);
	static void UpdateTextBlock(UTextBlockBase* const TextBlock, const FString& NewText);
};
