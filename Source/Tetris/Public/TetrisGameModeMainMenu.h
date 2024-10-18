// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisGameModeBase.h"
#include "TetrisGameModeMainMenu.generated.h"

class UTetrisWidgetMenuMain;

/**
 * 
 */
UCLASS()
class TETRIS_API ATetrisGameModeMainMenu : public ATetrisGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	virtual void Initialize() override;
	virtual void SetInputMode() override;
	virtual void InitializeEffect() override;

public:
	// Level Name
	static const FName MainMenuLevelName;

private:
	/** Widget References */
	UPROPERTY()
	TObjectPtr<UTetrisWidgetMenuMain> MainMenuWidget;

	/** Widget Class References */
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<UTetrisWidgetMenuMain> MainMenuWidgetClass;
};
