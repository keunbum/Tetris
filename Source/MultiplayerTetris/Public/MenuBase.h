// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBase.generated.h"

class UMenuButton;

UENUM()
enum class EMenuMoveDirection : uint8
{
	None = 0,
	Up = 1 << 0,
	Down = 1 << 1,
	Left = 1 << 2,
	Right = 1 << 3,
};

ENUM_CLASS_FLAGS(EMenuMoveDirection); 

/**
 *
 */
UCLASS()
class MULTIPLAYERTETRIS_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	/** UMenuBase */
    virtual void SetInitialFocus();

	void InitializeMenuButtons(const TArray<UMenuButton*>& InMenuButtons);
	void SetDefaultMenuButtonFocus();

	bool IsNoButtonFocused() const { return FocusedButtonIndex == InvalidButtonIndex; }

	/** static methods */
	static EMenuMoveDirection GetMenuMoveDirection(const FKey& Key);
	static bool IsMenuMoveDirectionValid(const EMenuMoveDirection MenuMoveDirection) { return MenuMoveDirection != EMenuMoveDirection::None; }
	static int32 GetMenuMoveDelta(const EMenuMoveDirection MenuMoveDirection) { return ((static_cast<int32>(MenuMoveDirection) & 1) == 0) ? -1 : 1; }

protected:
	void SetMenuButtonFocus(const int32 NewFocusedButtonIndex);
	void MoveMenuButtonFocus(const int32 MoveDelta);

private:
	/** static methods */
	static bool IsUpKey(const FKey& Key) { return (Key == EKeys::Up || Key == EKeys::Gamepad_DPad_Up || Key == EKeys::Gamepad_LeftStick_Up); }
	static bool IsDownKey(const FKey& Key) { return (Key == EKeys::Down || Key == EKeys::Gamepad_DPad_Down || Key == EKeys::Gamepad_LeftStick_Down); }
	static bool IsLeftKey(const FKey& Key) { return (Key == EKeys::Left || Key == EKeys::Gamepad_DPad_Left || Key == EKeys::Gamepad_LeftStick_Left); }
	static bool IsRightKey(const FKey& Key) { return (Key == EKeys::Right || Key == EKeys::Gamepad_DPad_Right || Key == EKeys::Gamepad_LeftStick_Right); }

protected:
	static constexpr int32 InvalidButtonIndex = -1;

	UPROPERTY()
	int32 DefaultFocusedButtonIndex;

	UPROPERTY()
	TArray<TObjectPtr<UMenuButton>> MenuButtons;

	UPROPERTY(VisibleAnywhere)
	int32 FocusedButtonIndex = 0;
};
