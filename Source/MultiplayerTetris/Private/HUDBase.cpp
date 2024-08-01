// Copyright Ryu KeunBeom. All Rights Reserved.

#include "HUDBase.h"

FString UHUDBase::GetFormattedTime(const float TimeInSeconds)
{
	// Convert to MM:SS.s
	const int32 TotalSeconds = FMath::FloorToInt(TimeInSeconds);
	const int32 Minutes = TotalSeconds / 60;
	const int32 Seconds = TotalSeconds % 60;
	const int32 Decimal = FMath::FloorToInt((TimeInSeconds - TotalSeconds) * 10.0f);
	return FString::Printf(TEXT("%02d:%02d.%d"), Minutes, Seconds, Decimal);
}
