// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "Board.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

#include "Mino.h"
#include "Tetrimino.h"

const FString ABoard::BackgroundMinoMaterialPath = TEXT("/Game/Material/M_MinoMaterial_Grey");
const FString ABoard::SpecialMinoMaterialPath = TEXT("/Game/Material/M_MinoMaterial_Black");

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set the default root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Initialize Class variables
	MinoClass = AMino::StaticClass();
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();
}

// Called every frame
void ABoard::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::Initialize()
{
	InitializeBackground();
	InitializeMinoMatrix();
}

void ABoard::InitializeBackground()
{
	UMaterialInterface* const BackgroundMinoMaterial = ABoard::GetMinoMaterialByPath(BackgroundMinoMaterialPath);
	if (!ensureMsgf(BackgroundMinoMaterial, TEXT("Failed to load material: %s"), *BackgroundMinoMaterialPath))
	{
		return;
	}

	Background.Reserve(TotalHeight * TotalWidth);
	for (int32 Row = 0; Row < TotalHeight; ++Row)
	{
		for (int32 Col = 0; Col < TotalWidth; ++Col)
		{
			AMino* const Mino = GetWorld()->SpawnActor<AMino>(MinoClass, FVector::ZeroVector, FRotator::ZeroRotator);
			check(Mino != nullptr);

			const FString& MinoMaterialPath = (Row == (TotalHeight - VisibleHeight) ? SpecialMinoMaterialPath : BackgroundMinoMaterialPath);
			UMaterialInterface* const MinoMaterial = ABoard::GetMinoMaterialByPath(MinoMaterialPath);
			check(MinoMaterial != nullptr);

			Mino->SetMaterial(MinoMaterial);

			Mino->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			const FIntVector2 MinoUnitPosition(Row, Col);
			Mino->SetRelativeLocationByIntVector2D(MinoUnitPosition);

			Background.Add(Mino);
		}
	}
}

void ABoard::InitializeMinoMatrix()
{
	MinoMatrix.Reserve(TotalHeight * TotalWidth);
	for (int32 Row = 0; Row < TotalHeight; ++Row)
	{
		for (int32 Col = 0; Col < TotalWidth; ++Col)
		{
			AMino* const Mino = nullptr;
			MinoMatrix.Add(Mino);
		}
	}
}

UMaterialInterface* ABoard::GetMinoMaterialByPath(const FString& Path)
{
	UMaterialInterface* const MinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *Path));
	return MinoMaterial;
}

