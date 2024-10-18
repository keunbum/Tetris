// Copyright Ryu KeunBeom. All Rights Reserved.


#include "Board.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "Algo/AllOf.h"
#include "Camera/CameraComponent.h"
#include "TetriminoBase.h"
#include "GameFramework/PlayerController.h"

const FName ABoard::WallMeshPath = TEXT("/Engine/BasicShapes/Plane");
const FMinoInfo ABoard::SpecialMinoInfo = FMinoInfo(TEXT("/Game/Material/M_MinoMaterial"), FLinearColor::White, 1.0f, 0);

ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = false;

	MinoClass = UMino::StaticClass();

	CreateBoardComponents();
	CreateMatrixWalls();

	CreateTestMinos();
}

void ABoard::Initialize()
{
	InitializeMinoMatrix();

	if (APlayerController* const PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetViewTarget(this);
	}
}

bool ABoard::IsDirectlyAboveSurface(const ATetrimino* Tetrimino) const
{
	static const FIntPoint MovementDown = ATetriminoBase::GetMatrixMovementIntPointByDirection(ATetrimino::MoveDirectionDown);
	return !IsMovementPossible(Tetrimino, MovementDown);
}

bool ABoard::IsBlocked(const ATetrimino* Tetrimino) const
{
	return !IsMovementPossible(Tetrimino, FIntPoint::ZeroValue);
}

bool ABoard::IsAboveSkyline(const ATetrimino* Tetrimino) const
{
	if (Tetrimino)
	{
		const FIntPoint& TetriminoMatrixLocation = Tetrimino->GetMatrixLocation();
		const TArray<FIntPoint>& MinoTetriminoLocalLocations = Tetrimino->GetMinoTetriminoLocalLocations();
		return Algo::AllOf(MinoTetriminoLocalLocations, [this, &TetriminoMatrixLocation](const FIntPoint& MinoTetriminoLocalLocation) {
			const FIntPoint MinoLocalMatrixLocation = TetriminoMatrixLocation + MinoTetriminoLocalLocation;
			return MinoLocalMatrixLocation.X < SkyLine;
			}
		);
	}

	return false;
}

bool ABoard::IsMovementPossible(const ATetrimino* Tetrimino, const FIntPoint& MovementIntPoint2D) const
{
	if (Tetrimino)
	{
		const FIntPoint NewTetriminoMatrixLocation = Tetrimino->GetMatrixLocation() + MovementIntPoint2D;
		const TArray<FIntPoint>& MinoTetriminoLocalLocations = Tetrimino->GetMinoTetriminoLocalLocations();
		return IsMinoLocationsPossible(MinoTetriminoLocalLocations, NewTetriminoMatrixLocation);
	}

	return false;
}

bool ABoard::IsRotationPossible(const ATetrimino* Tetrimino, const ETetriminoRotationDirection RotationDirection, const FIntPoint& RotationPointOffset) const
{
	if (Tetrimino)
	{
		const FIntPoint& NewTetriminoMatrixLocation = Tetrimino->GetMatrixLocation() + RotationPointOffset;
		const ETetriminoShape TetriminoShape = Tetrimino->GetShape();
		const ETetriminoFacing NewTetriminoFacing = Tetrimino->GetFacing() + static_cast<int32>(RotationDirection);
		const TArray<FIntPoint>& NewMinoLocalMatrixLocations = ATetriminoBase::GetMinoTetriminoLocalLocationsByTetriminoShapeAndFacing(TetriminoShape, NewTetriminoFacing);
		return IsMinoLocationsPossible(NewMinoLocalMatrixLocations, NewTetriminoMatrixLocation);
	}

	return false;
}

bool ABoard::IsRowFull(const int32 TargetRow) const
{
	for (int32 TargetCol = VisibleBeginCol; TargetCol < VisibleEndCol; ++TargetCol)
	{
		const FIntPoint TargetMatrixLocation(TargetRow, TargetCol);
		if (IsMatrixLocationEmpty(TargetMatrixLocation))
		{
			return false;
		}
	}
	return true;
}

void ABoard::AddMinos(const ATetrimino* Tetrimino)
{
	if (Tetrimino)
	{
		const FIntPoint TetriminoMatrixLocation = Tetrimino->GetMatrixLocation();
		const TArray<FIntPoint>& MinoTetriminoLocalLocations = Tetrimino->GetMinoTetriminoLocalLocations();
		const TArray<UMino*>& MinoArray = Tetrimino->GetMinoArray();
		for (int32 MinoIndex = 0; MinoIndex < Tetrimino->MinoNum; ++MinoIndex)
		{
			const FIntPoint& MinoTetriminoLocalLocation = MinoTetriminoLocalLocations[MinoIndex];
			const FIntPoint MinoMatrixLocation = TetriminoMatrixLocation + MinoTetriminoLocalLocation;
			UMino* const Mino = MinoArray[MinoIndex];
			AddMino(Mino, MinoMatrixLocation);
		}
	}
}

void ABoard::ClearRows(const TArray<int32>& TargetRows)
{
	// Clear Rows
	for (const int32 TargetRow : TargetRows)
	{
		ClearRow(TargetRow);
	}

	// Move Rows
	for (int32 Index = TargetRows.Num() - 1; Index >= 0; --Index)
	{
		static constexpr int32 LastEndRow = TotalBeginRow - 1;
		const int32 EmptyRow = TargetRows[Index];
		// [BeginRow, EndRow)
		const int32 BeginRow = EmptyRow - 1;
		const int32 EndRow = (Index - 1 >= 0 ? TargetRows[Index - 1] : LastEndRow);
		for (int32 Row = BeginRow; Row > EndRow; --Row)
		{
			const int32 MoveDistance = TargetRows.Num() - Index;
			MoveRow(Row, MoveDistance);
		}
	}
}

FIntPoint ABoard::GetFinalFallingMatrixLocation(const ATetrimino* Tetrimino) const
{
	if (Tetrimino)
	{
		FIntPoint FinalFallingMatrixLocation = Tetrimino->GetMatrixLocation();
		static const FIntPoint MovementDown = ATetriminoBase::GetMatrixMovementIntPointByDirection(ATetrimino::MoveDirectionDown);
		while (IsMinoLocationsPossible(Tetrimino->GetMinoTetriminoLocalLocations(), FinalFallingMatrixLocation + MovementDown))
		{
			FinalFallingMatrixLocation += MovementDown;
		}
		return FinalFallingMatrixLocation;
	}

	return FIntPoint::ZeroValue;
}

int32 ABoard::GetMatrixIndexByMatrixLocation(const FIntPoint& MatrixLocation)
{
	return TotalWidth * MatrixLocation.X + MatrixLocation.Y;
}

void ABoard::CreateBoardComponents()
{
	// 음수/양수 순서대로 FVector(우/좌, 아래/위, 생략)
	MatrixRelativeLocation = UMino::UnitLength * FVector(0.f, 0.f, 0.f);
	NextQueueRelativeLocation = UMino::UnitLength * FVector(-15.f, -15.f, -10.f);
	HoldQueueRelativeLocation = UMino::UnitLength * FVector(8.f, -15.f, -10.f);
	WallRelativeLocation = UMino::UnitLength * FVector(0, 0, 0);
	CameraRelativeLocation = UMino::UnitLength * FVector(-VisibleWidth / 2, -30.f, 31.f);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	MatrixRoot = CreateAndSetupSceneComponent(TEXT("MatrixRoot"), RootComponent, MatrixRelativeLocation);
	NextQueueRoot = CreateAndSetupSceneComponent(TEXT("NextQueueRoot"), MatrixRoot, NextQueueRelativeLocation);
	HoldQueueRoot = CreateAndSetupSceneComponent(TEXT("HoldQueueRoot"), MatrixRoot, HoldQueueRelativeLocation);
	WallRoot = CreateAndSetupSceneComponent(TEXT("WallRoot"), MatrixRoot, WallRelativeLocation);

	// Initialize and configure the camera component
	Camera = CreateAndSetupComponent<UCameraComponent>(TEXT("Camera"), MatrixRoot, CameraRelativeLocation);
	if (Camera)
	{
		Camera->SetRelativeRotation(FRotator(-90.f, 0.f, 90.f));
	}
}

void ABoard::CreateMatrixWalls()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallMesh(*ABoard::WallMeshPath.ToString());
	if (!ensureMsgf(WallMesh.Succeeded(), TEXT("Failed to Load WallMesh")))
	{
		return;
	}

	static constexpr int32 WallNum = 4;
	Walls.Reserve(WallNum);
	for (const TCHAR* ComponentName : { TEXT("LeftWall"), TEXT("RightWall"), TEXT("BackWall"), TEXT("Floor") })
	{
		if (UStaticMeshComponent* const Wall = CreateDefaultSubobject<UStaticMeshComponent>(ComponentName))
		{
			Wall->SetupAttachment(WallRoot);
			Wall->SetStaticMesh(WallMesh.Object);
			Walls.Add(Wall);
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

void ABoard::CreateTestMinos()
{
	// Matrix의 보이는 영역을 채울 테스트용 미노들을 생성한다.
	for (int32 Row = VisibleBeginRow; Row < VisibleEndRow; ++Row)
	{
		const FMinoInfo& MinoInfo = SpecialMinoInfo;
		for (int32 Col = VisibleBeginCol; Col < VisibleEndCol; ++Col)
		{
			const FIntPoint MinoMatrixLocation(Row, Col);
			static constexpr float Z = 0.f;
			const FString MinoName = FString::Printf(TEXT("Mino_%d_%d"), Row, Col);
			if (UMino* const Mino = CreateDefaultSubobject<UMino>(*MinoName))
			{
				if (UMaterialInterface* const MinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *MinoInfo.MaterialPath)))
				{
					static constexpr int32 ElementIndex = 0;
					Mino->SetMaterial(ElementIndex, MinoMaterial);
				}
				Mino->SetTranslucentSortPriority(-2);
				Mino->AttachToWithMatrixLocation(MatrixRoot, MinoMatrixLocation, Z);
			}
		}
	}
}

UMino* ABoard::GetMinoByMatrixLocation(const FIntPoint& MatrixLocation) const
{
	const int32 Index = ABoard::GetMatrixIndexByMatrixLocation(MatrixLocation);
	return MinoMatrix[Index];
}

void ABoard::SetMinoByMatrixLocation(UMino* const Mino, const FIntPoint& MatrixLocation)
{
	const int32 Index = ABoard::GetMatrixIndexByMatrixLocation(MatrixLocation);
	MinoMatrix[Index] = Mino;
}

bool ABoard::IsMatrixLocationEmpty(const FIntPoint& MatrixLocation) const
{
	const UMino* Mino = GetMinoByMatrixLocation(MatrixLocation);
	return !IsValid(Mino);
}

bool ABoard::IsMinoLocationsPossible(const TArray<FIntPoint>& MinoTetriminoLocalLocations, const FIntPoint& TetriminoMatrixLocation) const
{
	return Algo::AllOf(MinoTetriminoLocalLocations, [this, &TetriminoMatrixLocation](const FIntPoint& MinoTetriminoLocalLocation) {
		const FIntPoint NewMinoLocalMatrixLocation = TetriminoMatrixLocation + MinoTetriminoLocalLocation;
		return FMath::IsWithin(NewMinoLocalMatrixLocation.X, TotalBeginRow, VisibleEndRow)
			&& FMath::IsWithin(NewMinoLocalMatrixLocation.Y, VisibleBeginCol, VisibleEndCol)
			&& (IsMatrixLocationEmpty(NewMinoLocalMatrixLocation));
		}
	);
}

void ABoard::AddMino(UMino* const Mino, const FIntPoint& MinoMatrixLocation)
{
	// Change ownership of the component to the board
	if (Mino)
	{
		Mino->Rename(nullptr, this);
		Mino->AttachToWithMatrixLocation(MatrixRoot, MinoMatrixLocation);
		SetMinoByMatrixLocation(Mino, MinoMatrixLocation);
	}
}

void ABoard::ClearRow(const int32 TargetRow)
{
	for (int32 TargetCol = VisibleBeginCol; TargetCol < VisibleEndCol; ++TargetCol)
	{
		const FIntPoint MinoMatrixLocation(TargetRow, TargetCol);
		UMino* const Mino = GetMinoByMatrixLocation(MinoMatrixLocation);
		RemoveMino(Mino, MinoMatrixLocation);
	}
}

void ABoard::MoveRow(const int32 TargetRow, const int32 MoveDistance)
{
	for (int32 TargetCol = VisibleBeginCol; TargetCol < VisibleEndCol; ++TargetCol)
	{
		const FIntPoint MinoMatrixLocation(TargetRow, TargetCol);
		if (!IsMatrixLocationEmpty(MinoMatrixLocation))
		{
			UMino* const Mino = GetMinoByMatrixLocation(MinoMatrixLocation);
			const FIntPoint NewMinoMatrixLocation(/* NewRow */ TargetRow + MoveDistance, TargetCol);
			MoveMino(Mino, /* OldMatrixLocation */ MinoMatrixLocation, NewMinoMatrixLocation);
		}
	}
}

void ABoard::RemoveMino(UMino* const Mino, const FIntPoint& MinoMatrixLocation)
{
	if (Mino)
	{
		Mino->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Mino->DestroyComponent();
		SetMinoByMatrixLocation(nullptr, MinoMatrixLocation);
	}
}

void ABoard::MoveMino(UMino* const Mino, const FIntPoint& OldMatrixLocation, const FIntPoint& NewMatrixLocation)
{
	if (Mino)
	{
		Mino->SetRelativeLocationByMatrixLocation(NewMatrixLocation);
		SetMinoByMatrixLocation(Mino, NewMatrixLocation);
		SetMinoByMatrixLocation(nullptr, OldMatrixLocation);
	}
}
