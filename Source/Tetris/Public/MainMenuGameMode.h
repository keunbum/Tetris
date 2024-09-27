// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisGameModeBase.h"
#include "MainMenuGameMode.generated.h"

class UMainMenuWidget;

/**
 * 
 */
UCLASS()
class TETRIS_API AMainMenuGameMode : public ATetrisGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	virtual void Initialize() override;
	virtual void SetInputMode() override;
	virtual void InitializeDefaultEffect() override;

public:
	// Level Name
	static const FName MainMenuLevelName;

private:
	/** Widget References */
	UPROPERTY()
	TObjectPtr<UMainMenuWidget> MainMenuWidget;

	/** Widget Class References */
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
};
