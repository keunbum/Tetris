// Copyright Ryu KeunBeom. All Rights Reserved.


#include "TetrisHudIngameBase.h"
#include "TetrisPlayerState.h"
#include "TetrisGameModeIngameBase.h"
#include "Engine/World.h"
#include "HUDSingle.h"

void ATetrisHudIngameBase::BeginPlay()
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

	HudWidget = CreateWidget<UHUDSingle>(GetWorld(), HudWidgetClass);
	if (!ensureMsgf(HudWidget, TEXT("HudWidget is nullptr")))
	{
		return;
	}
	HudWidget->InitializeDisplay(TetrisPlayerState->GetHUDSingleUpdateDisplayParams());
	HudWidget->AddToViewport();

	// 경과 시간 업데이트 타이머 시작
	FTimerHandle UpdateTimeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(UpdateTimeTimerHandle, this, &ATetrisHudIngameBase::OnUpdateTime, ATetrisHudIngameBase::TimeDisplayUpdateInterval, /* InbLoop */ true, /* InFirstDelay */ 0.0f);
}

void ATetrisHudIngameBase::Update()
{
	if (TetrisPlayerState && HudWidget)
	{
		HudWidget->UpdateDisplay(TetrisPlayerState->GetHUDSingleUpdateDisplayParams());
	}
}

void ATetrisHudIngameBase::OnUpdateTime()
{
	if (TetrisGameMode && HudWidget)
	{
		const float ElapsedTime = TetrisGameMode->GetElapsedTime();
		HudWidget->UpdateTimeDisplay(ElapsedTime);
	}
}
