// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "TetrisGameModeBase.h"

#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerTetrisGameModeBase::AMultiplayerTetrisGameModeBase()
{
}

void AMultiplayerTetrisGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void AMultiplayerTetrisGameModeBase::Initialize()
{

}
