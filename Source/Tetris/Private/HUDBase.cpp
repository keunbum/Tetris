// Copyright Ryu KeunBeom. All Rights Reserved.

#include "HUDBase.h"
#include "TextBlockBase.h"

FString UHUDBase::GetFormattedTimeString(const float TimeInSeconds)
{
	// Convert to MM:SS.s
	const int32 TotalSeconds = FMath::FloorToInt(TimeInSeconds);
	const int32 Minutes = TotalSeconds / 60;
	const int32 Seconds = TotalSeconds % 60;
	const int32 Decimal = FMath::FloorToInt((TimeInSeconds - TotalSeconds) * 10.0f);
	return FString::Printf(TEXT("%02d:%02d.%d"), Minutes, Seconds, Decimal);
}

void UHUDBase::UpdateTextBlockWithNameAndValue(UTextBlockBase* const TextBlock, const FString& Name, const FString& Value)
{
	static constexpr int32 TotalStringLength = 35;
	const int32 Margin = TotalStringLength - (Name.Len() + Value.Len());
	ensureMsgf(Margin >= 0, TEXT("TextBlock Name: %s, Value: %s"), *Name, *Value);
	const FString Text = FString::Printf(TEXT("%s%*s"), *Name, Margin, *Value);
	UpdateTextBlock(TextBlock, Text);
}

void UHUDBase::UpdateTextBlock(UTextBlockBase* const TextBlock, const FString& NewText)
{
	if (TextBlock)
	{
		TextBlock->SetText(FText::FromString(NewText));
	}
}
