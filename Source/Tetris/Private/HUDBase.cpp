// Copyright Ryu KeunBeom. All Rights Reserved.

#include "HUDBase.h"
#include "TextBlockBase.h"

FString UHUDBase::GetFormattedTimeString(const float TimeInSeconds)
{
	// Convert to MM:SS
	const int32 TotalSeconds = FMath::FloorToInt(TimeInSeconds);
	const int32 Minutes = TotalSeconds / 60;
	const int32 Seconds = TotalSeconds % 60;
	return FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
}

void UHUDBase::UpdateTextBlockWithNameAndValue(UTextBlockBase* const TextBlock, const FString& Name, const FString& Value)
{
	static constexpr int32 TotalStringLength = 35;
	const int32 Margin = TotalStringLength - (Name.Len() + Value.Len());
	ensureMsgf(Margin >= 0, TEXT("TextBlock Name: %s, Value: %s"), *Name, *Value);
	const FString Text = FString::Printf(TEXT("%s%*s"), *Name, Margin, *Value);
	UHUDBase::UpdateTextBlock(TextBlock, Text);
}

void UHUDBase::UpdateTextBlock(UTextBlockBase* const TextBlock, const FString& NewText)
{
	if (TextBlock)
	{
		TextBlock->SetText(FText::FromString(NewText));
	}
}
