// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidgetBase.generated.h"

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
class MULTIPLAYERTETRIS_API UMenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuButtons(const TArray<UMenuButton*>& InMenuButtons);
	void SetDefaultMenuButtonFocus();
	void SetWidgetFocusOnly();

	bool IsNoButtonFocused() const { return FocusedButtonIndex == InvalidButtonIndex; }

	/** static methods */
	static bool GetMenuMoveDirection(const FKey& Key, EMenuMoveDirection& OutMenuMoveDirection);
	static bool IsMenuMoveDirectionValid(const EMenuMoveDirection MenuMoveDirection) { return MenuMoveDirection != EMenuMoveDirection::None; }
	static int32 GetMenuMoveDelta(const EMenuMoveDirection MenuMoveDirection) { return ((static_cast<int32>(MenuMoveDirection) & 1) == 0) ? -1 : 1; }

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void UpdateMenuButtonFocus(const int32 NewFocusedButtonIndex);
	void MoveMenuButtonFocus(const int32 MoveDelta);

private:
	/** UMenuWidgetBase */
	virtual void SetInitialFocus() PURE_VIRTUAL(UMenuWidgetBase::SetInitialFocus, );

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
