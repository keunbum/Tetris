// Copyright © 2024 Ryu KeunBeom. All Rights Reserved.


#include "Board.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "Algo/AllOf.h"

#include "TetriminoBase.h"

const FMinoInfo ABoard::BackgroundMinoInfo = FMinoInfo(TEXT("/Game/Material/M_MinoMaterial"), FLinearColor::Gray);
const FMinoInfo ABoard::SpecialMinoInfo = FMinoInfo(TEXT("/Game/Material/M_MinoMaterial"), FLinearColor::Black);

ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = false;

	// 음수/양수 순서대로 FVector(우/좌, 아래/위, 생략)
	NextQueueRelativeLocation = FVector(UMino::UnitLength * -12.f, UMino::UnitLength * -15.f, 0.f);
	HoldQueueRelativeLocation = FVector(UMino::UnitLength * 4.f, UMino::UnitLength * -15.f, 0.f);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	check(RootComponent != nullptr);

	MinoClass = UMino::StaticClass();

	/** MatrixRoot */
	MatrixRoot = CreateAndSetupSceneComponent(TEXT("MatrixRoot"), RootComponent);

	/** BackgroundRoot */
	BackgroundRoot = CreateAndSetupSceneComponent(TEXT("BackgroundRoot"), RootComponent);

	/** NextQueueRoot */
	NextQueueRoot = CreateAndSetupSceneComponent(TEXT("NextQueueRoot"), RootComponent);
	NextQueueRoot->SetRelativeLocation(NextQueueRelativeLocation);

	/** HoldQueueRoot */
	HoldQueueRoot = CreateAndSetupSceneComponent(TEXT("HoldQueueRoot"), RootComponent);
	HoldQueueRoot->SetRelativeLocation(HoldQueueRelativeLocation);
}

void ABoard::Initialize()
{
	InitializeBackground();
	InitializeMinoMatrix();
}

bool ABoard::IsMovementPossible(const ATetrimino* Tetrimino, const FIntPoint& MovementIntPoint2D) const
{
	check(Tetrimino != nullptr);
	const FIntPoint NewTetriminoMatrixLocation = Tetrimino->GetMatrixLocation() + MovementIntPoint2D;
	const TArray<FIntPoint>& MinoMatrixLocalLocations = Tetrimino->GetMinoMatrixLocalLocations();
	return IsMinoLocationsPossible(MinoMatrixLocalLocations, NewTetriminoMatrixLocation);
}

bool ABoard::IsRotationPossible(const ATetrimino* Tetrimino, const ETetriminoRotationDirection RotationDirection, const FIntPoint& RotationPointOffset) const
{
	check(Tetrimino != nullptr);
	const FIntPoint& NewTetriminoMatrixLocation = Tetrimino->GetMatrixLocation() + RotationPointOffset;
	const ETetriminoShape TetriminoShape = Tetrimino->GetShape();
	const ETetriminoFacing NewTetriminoFacing = Tetrimino->GetFacing() + static_cast<int32>(RotationDirection);
	const TArray<FIntPoint>& NewMinoLocalMatrixLocations = ATetriminoBase::GetMinoMatrixLocalLocationsByTetriminoShapeAndFacing(TetriminoShape, NewTetriminoFacing);
	return IsMinoLocationsPossible(NewMinoLocalMatrixLocations, NewTetriminoMatrixLocation);
}

void ABoard::AddMinos(const ATetrimino* Tetrimino)
{
	check(Tetrimino != nullptr);

	const FIntPoint TetriminoMatrixLocation = Tetrimino->GetMatrixLocation();
	const TArray<FIntPoint>& MinoMatrixLocalLocations = Tetrimino->GetMinoMatrixLocalLocations();
	const TArray<UMino*>& MinoArray = Tetrimino->GetMinoArray();
	for (int32 MinoIndex = 0; MinoIndex < Tetrimino->MinoNum; ++MinoIndex)
	{
		const FIntPoint& MinoMatrixLocalLocation = MinoMatrixLocalLocations[MinoIndex];
		const FIntPoint MinoMatrixLocation = TetriminoMatrixLocation + MinoMatrixLocalLocation;
		UMino* const Mino = MinoArray[MinoIndex];
		// Change ownership of the component to the board
		Mino->Rename(nullptr, this);
		Mino->AttachToWithMatrixLocation(MatrixRoot, MinoMatrixLocation);
		SetMinoByMatrixLocation(Mino, MinoMatrixLocation);
	}
}

FIntPoint ABoard::GetFinalFallingMatrixLocation(const ATetrimino* Tetrimino) const
{
	check(Tetrimino != nullptr);

	FIntPoint FinalFallingMatrixLocation = Tetrimino->GetMatrixLocation();
	static const FIntPoint MovementIntPoint2D = ATetriminoBase::GetMatrixMovementIntPointByDirection(ATetrimino::MoveDirectionDown);
	while (IsMinoLocationsPossible(Tetrimino->GetMinoMatrixLocalLocations(), FinalFallingMatrixLocation + MovementIntPoint2D))
	{
		FinalFallingMatrixLocation += MovementIntPoint2D;
	}
	return FinalFallingMatrixLocation;
}

void ABoard::InitializeBackground()
{
	for (int32 Row = 0; Row < TotalHeight; ++Row)
	{
		const FMinoInfo& MinoInfo = (Row == (TotalHeight - VisibleHeight) ? SpecialMinoInfo : BackgroundMinoInfo);
		for (int32 Col = 0; Col < TotalWidth; ++Col)
		{
			const FIntPoint MinoMatrixLocation(Row, Col);
			static constexpr float Z = 0.f - UMino::UnitLength;
			UMino* const Mino = UMino::CreateMino(this, MinoInfo);
			check(Mino != nullptr);
			Mino->AttachToWithMatrixLocation(BackgroundRoot, MinoMatrixLocation, Z);
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

int32 ABoard::GetMatrixIndexByMatrixLocation(const FIntPoint& MatrixLocation) const
{
	return TotalWidth * MatrixLocation.X + MatrixLocation.Y;
}

UMino* ABoard::GetMinoByMatrixLocation(const FIntPoint& MatrixLocation) const
{
	const int32 Index = GetMatrixIndexByMatrixLocation(MatrixLocation);
	return MinoMatrix[Index];
}

void ABoard::SetMinoByMatrixLocation(UMino* const Mino, const FIntPoint& MatrixLocation)
{
	const int32 Index = GetMatrixIndexByMatrixLocation(MatrixLocation);
	MinoMatrix[Index] = Mino;
}

bool ABoard::IsMatrixLocationEmpty(const FIntPoint& MatrixLocation) const
{
	const UMino* Mino = GetMinoByMatrixLocation(MatrixLocation);
	return !IsValid(Mino);
}

bool ABoard::IsMinoLocationsPossible(const TArray<FIntPoint>& MinoLocalMatrixLocations, const FIntPoint& TetriminoMatrixLocation) const
{
	return Algo::AllOf(MinoLocalMatrixLocations, [this, &TetriminoMatrixLocation](const FIntPoint& MinoLocalMatrixLocation) {
		const FIntPoint NewMinoLocalMatrixLocation = TetriminoMatrixLocation + MinoLocalMatrixLocation;
		return FMath::IsWithin(NewMinoLocalMatrixLocation.X, TotalBeginRow, VisibleEndRow)
			&& FMath::IsWithin(NewMinoLocalMatrixLocation.Y, VisibleBeginCol, VisibleEndCol)
			&& (IsMatrixLocationEmpty(NewMinoLocalMatrixLocation));
		}
	);
}

USceneComponent* ABoard::CreateAndSetupSceneComponent(const FName& ComponentName, USceneComponent* const Parent)
{
	USceneComponent* const SceneComponent = CreateDefaultSubobject<USceneComponent>(ComponentName);
	check(SceneComponent != nullptr);
	SceneComponent->SetupAttachment(Parent);
	return SceneComponent;
}
