// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "Board.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "Algo/AllOf.h"

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
	UMaterialInterface* const BackgroundMinoMaterial = ABoard::GetMaterialByPath(BackgroundMinoMaterialPath);
	check(BackgroundMinoMaterial != nullptr);

	Background.Reserve(TotalHeight * TotalWidth);
	for (int32 Row = 0; Row < TotalHeight; ++Row)
	{
		for (int32 Col = 0; Col < TotalWidth; ++Col)
		{
			AMino* const Mino = GetWorld()->SpawnActor<AMino>(MinoClass);
			check(Mino != nullptr);

			const FString& MinoMaterialPath = (Row == (TotalHeight - VisibleHeight) ? SpecialMinoMaterialPath : BackgroundMinoMaterialPath);
			UMaterialInterface* const MinoMaterial = ABoard::GetMaterialByPath(MinoMaterialPath);
			check(MinoMaterial != nullptr);

			static constexpr int32 ElementIndex = 0;
			Mino->SetMaterial(ElementIndex, MinoMaterial);

			Mino->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

			const FIntPoint MinoLocalMatrixLocation(Row, Col);
			Mino->SetRelativeLocationByMatrixLocation(MinoLocalMatrixLocation);

			Background.Add(Mino);
		}
	}
}

void ABoard::InitializeMinoMatrix()
{
	MinoMatrix.Reserve(TotalHeight * TotalWidth);
	for (int32 _ = 0; _ < TotalHeight * TotalWidth; ++_)
	{
		AMino* const Mino = nullptr;
		MinoMatrix.Add(Mino);

	}
}

AMino* ABoard::GetMinoByMatrixLocation(const FIntPoint& MatrixLocation) const
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

UMaterialInterface* ABoard::GetMaterialByPath(const FString& Path)
{
	UMaterialInterface* const MinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *Path));
	ensureMsgf(MinoMaterial != nullptr, TEXT("Failed to load material: %s"), *Path);
	return MinoMaterial;
}

UMaterialInterface* ABoard::GetMaterialByMinoInfo(const FMinoInfo& MinoInfo)
{
	UMaterialInterface* const MinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *MinoInfo.MaterialPath));
	ensureMsgf(MinoMaterial != nullptr, TEXT("Failed to load material: %s"), *MinoInfo.MaterialPath);
	return MinoMaterial;
}

UMaterialInstanceDynamic* ABoard::GetMaterialInstanceByMinoInfo(UObject* const InOuter, const FMinoInfo& MinoInfo)
{
	if (UMaterialInterface* const BaseMaterial = GetMaterialByMinoInfo(MinoInfo))
	{
		if (UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, InOuter))
		{
			DynamicMaterialInstance->SetVectorParameterValue("BaseColor", MinoInfo.Color);
			return DynamicMaterialInstance;
		}
	}
	return nullptr;
}
