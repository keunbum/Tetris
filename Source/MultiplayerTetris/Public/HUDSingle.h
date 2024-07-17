// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDSingle.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UHUDSingle : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateLevelDisplay(const int32 NewLevel);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;
};
