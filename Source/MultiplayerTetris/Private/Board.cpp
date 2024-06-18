// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "Board.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "Algo/AllOf.h"

#include "Tetrimino.h"

const FMinoInfo ABoard::BackgroundMinoInfo = FMinoInfo(TEXT("/Game/Material/M_MinoMaterial"), FLinearColor::Gray);
const FMinoInfo ABoard::SpecialMinoInfo = FMinoInfo(TEXT("/Game/Material/M_MinoMaterial"), FLinearColor::Black);

// Sets default values
ABoard::ABoard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set the default root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Initialize Class variables
	MinoClass = UMino::StaticClass();

	BackgroundRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BackgroundRoot"));
	BackgroundRoot->SetupAttachment(RootComponent);
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

bool ABoard::IsMovementPossible(const ATetrimino* Tetrimino, const FIntPoint& MovementIntPoint2D) const
{
	check(Tetrimino != nullptr);
	const FIntPoint NewTetriminoMatrixLocation = Tetrimino->GetMatrixLocation() + MovementIntPoint2D;
	const TArray<FIntPoint>& MinoLocalMatrixLocations = Tetrimino->GetMinoLocalMatrixLocations();
	return IsMinoLocationsPossible(NewTetriminoMatrixLocation, MinoLocalMatrixLocations);
}

bool ABoard::IsRotationPossible(const ATetrimino* Tetrimino, const ETetriminoRotationDirection RotationDirection, const FIntPoint& RotationPointOffset) const
{
	check(Tetrimino != nullptr);
	const FIntPoint& NewTetriminoMatrixLocation = Tetrimino->GetMatrixLocation() + RotationPointOffset;
	const ETetriminoShape TetriminoShape = Tetrimino->GetShape();
	const ETetriminoFacing NewTetriminoFacing = Tetrimino->GetFacing() + static_cast<int32>(RotationDirection);
	const TArray<FIntPoint>& NewMinoLocalMatrixLocations = ATetrimino::GetMinoLocalMatrixLocationsByTetriminoShapeAndFacing(TetriminoShape, NewTetriminoFacing);
	return IsMinoLocationsPossible(NewTetriminoMatrixLocation, NewMinoLocalMatrixLocations);
}

void ABoard::Initialize()
{
	InitializeBackground();
	InitializeMinoMatrix();
}

void ABoard::InitializeBackground()
{
	for (int32 Row = 0; Row < TotalHeight; ++Row)
	{
		const FMinoInfo& MinoInfo = (Row == (TotalHeight - VisibleHeight) ? SpecialMinoInfo : BackgroundMinoInfo);
		UMaterialInterface* const MinoMaterial = UMino::GetMaterialInstanceByMinoInfo(this, MinoInfo);
		check(MinoMaterial != nullptr);
		for (int32 Col = 0; Col < TotalWidth; ++Col)
		{
			UMino* Mino = NewObject<UMino>(this);
			check(Mino != nullptr);

			static constexpr int32 ElementIndex = 0;
			Mino->SetMaterial(ElementIndex, MinoMaterial);

			Mino->RegisterComponent();
			Mino->AttachToComponent(BackgroundRoot, FAttachmentTransformRules::KeepRelativeTransform);
			const FIntPoint MinoLocalMatrixLocation(Row, Col);
			Mino->SetRelativeLocationByMatrixLocation(MinoLocalMatrixLocation);
		}
	}
}

void ABoard::InitializeMinoMatrix()
{
	MinoMatrix.Reserve(TotalHeight * TotalWidth);
	for (int32 _ = 0; _ < TotalHeight * TotalWidth; ++_)
	{
		UMino* const Mino = nullptr;
		MinoMatrix.Add(Mino);
	}
}

UMino* ABoard::GetMinoByMatrixLocation(const FIntPoint& MatrixLocation) const
{
	const int32 Index = TotalWidth * MatrixLocation.X + MatrixLocation.Y;
	return MinoMatrix[Index];
}

bool ABoard::IsMatrixLocationEmpty(const FIntPoint& MatrixLocation) const
{
	return GetMinoByMatrixLocation(MatrixLocation) == nullptr;
}

bool ABoard::IsMinoLocationsPossible(const FIntPoint& TetriminoMatrixLocation, const TArray<FIntPoint>& MinoLocalMatrixLocations) const
{
	return Algo::AllOf(MinoLocalMatrixLocations, [this, &TetriminoMatrixLocation](const FIntPoint& MinoLocalMatrixLocation) {
		const FIntPoint NewMinoLocalMatrixLocation = TetriminoMatrixLocation + MinoLocalMatrixLocation;
		return FMath::IsWithin(NewMinoLocalMatrixLocation.X, TotalBeginRow, VisibleEndRow)
			&& FMath::IsWithin(NewMinoLocalMatrixLocation.Y, VisibleBeginCol, VisibleEndCol)
			&& (IsMatrixLocationEmpty(NewMinoLocalMatrixLocation));
		}
	);
}
