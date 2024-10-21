// Copyright Ryu KeunBeom. All Rights Reserved.


#include "TetrisHudIngame.h"
#include "TetrisPlayerState.h"
#include "TetrisGameModeIngameBase.h"
#include "Engine/World.h"
#include "TetrisWidgetHudIngame.h"

void ATetrisHudIngame::BeginPlay()
{
	Super::BeginPlay();

	TetrisGameMode = Cast<ATetrisGameModeIngameBase>(GetWorld()->GetAuthGameMode());
	if (!ensureMsgf(TetrisGameMode, TEXT("TetrisGameMode is nullptr")))
	{
		return;
	}

	TetrisPlayerState = TetrisGameMode->GetTetrisPlayerState();
	if (!ensureMsgf(TetrisPlayerState, TEXT("TetrisPlayerState is nullptr")))
	{
		return;
	}

	HudWidget = CreateWidget<UTetrisWidgetHudIngame>(GetWorld(), HudWidgetClass);
	if (!ensureMsgf(HudWidget, TEXT("HudWidget is nullptr")))
	{
		return;
	}
	HudWidget->InitializeDisplay(TetrisPlayerState->GetHudIngameUpdateDisplayParams());
	HudWidget->AddToViewport();



	// 경과 시간 업데이트 타이머 시작
	FTimerHandle UpdateTimeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(UpdateTimeTimerHandle, this, &ATetrisHudIngame::OnUpdateTime, ATetrisHudIngame::TimeDisplayUpdateInterval, /* InbLoop */ true, /* InFirstDelay */ 0.0f);
}

void ATetrisHudIngame::Update()
{
	if (TetrisPlayerState && HudWidget)
	{
		HudWidget->UpdateDisplay(TetrisPlayerState->GetHudIngameUpdateDisplayParams());
	}
}

void ATetrisHudIngame::OnUpdateTime()
{
	if (TetrisGameMode && HudWidget)
	{
		const float ElapsedTime = TetrisGameMode->GetElapsedTime();
		HudWidget->UpdateTimeDisplay(ElapsedTime);
	}
}
