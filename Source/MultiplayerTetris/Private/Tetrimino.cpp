// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#include "Tetrimino.h"

#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

#include "Mino.h"
#include "Board.h"

const FVector2D ATetrimino::MoveDirectionLeft = FVector2D(0, -1);
const FVector2D ATetrimino::MoveDirectionRight = -MoveDirectionLeft;
const FVector2D ATetrimino::MoveDirectionDown = MoveDirectionLeft.GetRotated(90.f);

const TMap<ETetriminoShape, FTetriminoShapeInfo> ATetrimino::TetriminoShapeInfos =
{
	{
		ETetriminoShape::O,
		{
			{
				{ ETetriminoFacing::North, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::East, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::South, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::West, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } }
			},
			TEXT("/Game/Material/M_MinoMaterial"),
			FLinearColor::Yellow,
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY),
			{
				{
					ETetriminoFacing::North,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0} } },
					}
				},
				{
					ETetriminoFacing::East,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0} } },
					}
				},
				{
					ETetriminoFacing::South,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0} } },
					}
				},
				{
					ETetriminoFacing::West,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0} } },
					}
				}
			},
		}
	},
	{
		ETetriminoShape::I,
		{
			{
				{ ETetriminoFacing::North, { {1, 0}, {1, 1}, {1, 2}, {1, 3} } },
				{ ETetriminoFacing::East, { {0, 2}, {1, 2}, {2, 2}, {3, 2} } },
				{ ETetriminoFacing::South, { {2, 0}, {2, 1}, {2, 2}, {2, 3} } },
				{ ETetriminoFacing::West, { {0, 1}, {1, 1}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial"),
			FLinearColor(0.0f, 1.0f, 1.0f),
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 1, ABoard::TetriminoDefaultSpawnLocationY),
			{
				{
					ETetriminoFacing::North,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, -1}, {0, 2}, {-2, -1}, {1, 2} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, -2}, {0, 1}, {1, -2}, {-2, 1} } },
					}
				},
				{
					ETetriminoFacing::East,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, 2}, {0, -1}, {-1, 2}, {-2, -1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, -1}, {0, 2}, {-2, -1}, {1, 2} } },
					}
				},
				{
					ETetriminoFacing::South,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, 1}, {0, -2}, {2, 1}, {-1, -2} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, 2}, {0, -1}, {-1, 2}, {2, -1} } },
					}
				},
				{
					ETetriminoFacing::West,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, -2}, {0, 1}, {1, -2}, {-2, 1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, 1}, {0, -2}, {2, 1}, {-1, -2} } },
					}
				}
			},
		}
	},
	{
		ETetriminoShape::T,
		{
			{
				{ ETetriminoFacing::North, { {1, 1}, {2, 0}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::East, { {1, 1}, {2, 1}, {2, 2}, {3, 1} } },
				{ ETetriminoFacing::South, { {2, 0}, {2, 1}, {2, 2}, {3, 1} } },
				{ ETetriminoFacing::West, { {1, 1}, {2, 0}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial"),
			FLinearColor(0.5f, 0.0f, 0.5f),
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY),
			{
				{
					ETetriminoFacing::North,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, 1}, {-1, 1}, {2, 1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, -1}, {-1, -1}, {2, -1} } }
					}
				},
				{
					ETetriminoFacing::East,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1} } },
					}
				},
				{
					ETetriminoFacing::South,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, -1}, {2, 0}, {2, -1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, 1}, {2, 0}, {2, 1} } },
					}
				},
				{
					ETetriminoFacing::West,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1} } },
					}
				}
			},
		}
	},
	{
		ETetriminoShape::L,
		{
			{
				{ ETetriminoFacing::North, { {1, 2}, {2, 0}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::East, { {1, 1}, {2, 1}, {3, 1}, {3, 2} } },
				{ ETetriminoFacing::South, { {2, 0}, {2, 1}, {2, 2}, {3, 0} } },
				{ ETetriminoFacing::West, { {1, 0}, {1, 1}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial"),
			FLinearColor(1.0f, 0.5f, 0.0f),
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY),
			{
				{
					ETetriminoFacing::North,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, 1}, {-1, 1}, {2, 0}, {2, 1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, -1}, {-1, -1}, {2, 0}, {2, -1} } },
					}
				},
				{
					ETetriminoFacing::East,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1} } },
					}
				},
				{
					ETetriminoFacing::South,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, -1}, {-1, -1}, {2, 0}, {2, -1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, 1}, {-1, 1}, {2, 0}, {2, 1} } },
					}
				},
				{
					ETetriminoFacing::West,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1} } },
					}
				}
			},
		}
	},
	{
		ETetriminoShape::J,
		{
			{
				{ ETetriminoFacing::North, { {1, 0}, {2, 0}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::East, { {1, 1}, {1, 2}, {2, 1}, {3, 1} } },
				{ ETetriminoFacing::South, { {2, 0}, {2, 1}, {2, 2}, {3, 2} } },
				{ ETetriminoFacing::West, { {1, 1}, {2, 1}, {3, 0}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial"),
			FLinearColor::Blue,
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY),
			{
				{
					ETetriminoFacing::North,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, 1}, {-1, 1}, {2, 0}, {2, 1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, -1}, {-1, -1}, {2, 0}, {2, -1} } },
					}
				},
				{
					ETetriminoFacing::East,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1} } },
					}
				},
				{
					ETetriminoFacing::South,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, -1}, {-1, -1}, {2, 0}, {2, -1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, 1}, {-1, 1}, {2, 0}, {2, 1} } },
					}
				},
				{
					ETetriminoFacing::West,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1} } },
					}
				}
			},
		}
	},
	{
		ETetriminoShape::S,
		{
			{
				{ ETetriminoFacing::North, { {1, 1}, {1, 2}, {2, 0}, {2, 1} } },
				{ ETetriminoFacing::East, { {1, 1}, {2, 1}, {2, 2}, {3, 2} } },
				{ ETetriminoFacing::South, { {2, 1}, {2, 2}, {3, 0}, {3, 1} } },
				{ ETetriminoFacing::West, { {1, 0}, {2, 0}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial"),
			FLinearColor::Green,
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY),
			{
				{
					ETetriminoFacing::North,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, 1}, {-1, 1}, {2, 0}, {2, 1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, -1}, {-1, -1}, {2, 0}, {2, -1} } },
					}
				},
				{
					ETetriminoFacing::East,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1} } },
					}
				},
				{
					ETetriminoFacing::South,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, -1}, {-1, -1}, {2, 0}, {2, -1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, 1}, {-1, 1}, {2, 0}, {2, 1} } },
					}
				},
				{
					ETetriminoFacing::West,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1} } },
					}
				}
			},
		}
	},
	{
		ETetriminoShape::Z,
		{
			{
				{ ETetriminoFacing::North, { {1, 0}, {1, 1}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::East, { {1, 2}, {2, 1}, {2, 2}, {3, 1} } },
				{ ETetriminoFacing::South, { {2, 0}, {2, 1}, {3, 1}, {3, 2} } },
				{ ETetriminoFacing::West, { {1, 1}, {2, 0}, {2, 1}, {3, 0} } }
			},
			TEXT("/Game/Material/M_MinoMaterial"),
			FLinearColor::Red,
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY),
			{
				{
					ETetriminoFacing::North,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, 1}, {-1, 1}, {2, 0}, {2, 1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, -1}, {-1, -1}, {2, 0}, {2, -1} } },
					}
				},
				{
					ETetriminoFacing::East,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1} } },
					}
				},
				{
					ETetriminoFacing::South,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, -1}, {-1, -1}, {2, 0}, {2, -1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, 1}, {-1, 1}, {2, 0}, {2, 1} } },
					}
				},
				{
					ETetriminoFacing::West,
					{
						{ ETetriminoRotationDirection::CounterClockwise, { {0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1} } },
						{ ETetriminoRotationDirection::Clockwise, { {0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1} } },
					}
				}
			},
		}
	}
};

ATetrimino::ATetrimino()
	: Shape(ETetriminoShape::None)
	, Facing(ETetriminoFacing::North)
	, MatrixLocation(FIntPoint(0, 0))
	, MinoClass(AMino::StaticClass())
	, MinoArray()
	, bIsGhostPieceOn(true)
	, GhostPiece(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void ATetrimino::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATetrimino::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const TArray<FIntPoint>& ATetrimino::GetMinoLocalMatrixLocations() const
{
	return ATetrimino::GetMinoLocalMatrixLocationsByTetriminoShapeAndFacing(Shape, Facing);
}

const TArray<FIntPoint>& ATetrimino::GetSRSRotationPointOffsets(const ETetriminoRotationDirection RotationDirection) const
{
	const FRotationInfo RotationInfo(Shape, Facing, RotationDirection);
	return ATetrimino::GetSRSRotationPointOffsetsByRotationInfo(RotationInfo);
}

const FTetriminoShapeInfo& ATetrimino::GetTetriminoShapeInfo() const
{
	return ATetrimino::GetTetriminoShapeInfoByShape(Shape);
}

const FIntPoint& ATetrimino::GetInitialMatrixLocation() const
{
	return ATetrimino::GetInitialMatrixLocationByShape(Shape);
}

void ATetrimino::Initialize(const ETetriminoShape NewTetriminoShape)
{
	SetShape(NewTetriminoShape);

	InitializeMinoArray();
}

void ATetrimino::MoveBy(const FIntPoint& IntPoint2D)
{
	const FVector LocalOffset(AMino::Get3DRelativePositionByMatrixLocation(IntPoint2D));
	AddActorLocalOffset(LocalOffset);
	MatrixLocation += IntPoint2D;
}

void ATetrimino::RotateTo(const ETetriminoRotationDirection RotationDirection)
{
	SetFacing(Facing + static_cast<int32>(RotationDirection));
	UpdateMinoLocalMatrixLocations();
}

void ATetrimino::AttachToBoard(ABoard* const Board)
{
	AttachToActor(Board, FAttachmentTransformRules::KeepRelativeTransform);
	MoveBy(GetInitialMatrixLocation());
}

void ATetrimino::DebugPrintState() const
{
	UE_LOG(LogTemp, Log, TEXT("TetriminoInPlay Type: %s"), *GetTetriminoShapeName(Shape));
	//UE_LOG(LogTemp, Log, TEXT("Tetrimino: Location: %s"), *GetActorLocation().ToString());
	UE_LOG(LogTemp, Log, TEXT("Tetrimino: Facing: %s"), *GetFacingName(Facing));

	//for (int32 Index = 0; Index < MinoNum; ++Index)
	//{
	//	if (MinoArray[Index])
	//	{
	//		const FVector MinoRelativeLocation = MinoArray[Index]->GetRelativeLocation();
	//		UE_LOG(LogTemp, Log, TEXT("Mino %d: Relative Location: %s"), Index, *MinoRelativeLocation.ToString());
	//	}
	//}
}

ETetriminoShape ATetrimino::GetTetriminoShapeRandom()
{
	const int32 RandomIndex = FMath::RandRange(0, static_cast<int32>(ETetriminoShape::Max) - 1);
	const ETetriminoShape TetriminoShape = static_cast<ETetriminoShape>(RandomIndex);
	return TetriminoShape;
}

const TArray<FIntPoint>& ATetrimino::GetMinoLocalMatrixLocationsByTetriminoShapeAndFacing(const ETetriminoShape Shape, const ETetriminoFacing Facing)
{
	const FTetriminoShapeInfo& TetriminoShapeInfo = ATetrimino::GetTetriminoShapeInfoByShape(Shape);
	return TetriminoShapeInfo.MinoLocalMatrixLocationsByFacing[Facing];
}

UMaterialInstanceDynamic* ATetrimino::GetMaterialInstance()
{
	const FTetriminoShapeInfo& TetriminoShapeInfo = GetTetriminoShapeInfo();
	return AMino::GetMaterialInstanceByMinoInfo(this, FMinoInfo(TetriminoShapeInfo.MaterialPath, TetriminoShapeInfo.Color));
}

void ATetrimino::InitializeMinoArray()
{
	UMaterialInterface* const MinoMaterial = GetMaterialInstance();
	check(MinoMaterial != nullptr);

	const TArray<FIntPoint>& MinoLocalMatrixLocations = GetMinoLocalMatrixLocations();
	check(MinoLocalMatrixLocations.Num() == MinoNum);

	MinoArray.Reserve(MinoNum);
	for (const FIntPoint& MinoLocalMatrixLocation : MinoLocalMatrixLocations)
	{
		if (AMino* const Mino = GetWorld()->SpawnActor<AMino>(MinoClass, FVector::ZeroVector, FRotator::ZeroRotator))
		{
			static constexpr int32 ElementIndex = 0;
			Mino->SetMaterial(ElementIndex, MinoMaterial);

			Mino->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			Mino->SetRelativeLocationByMatrixLocation(MinoLocalMatrixLocation);

			MinoArray.Add(Mino);
		}
	}
}

void ATetrimino::UpdateMinoLocalMatrixLocations()
{
	const TArray<FIntPoint>& MinoLocalMatrixLocations = GetMinoLocalMatrixLocations();
	for (int32 MinoID = 0; MinoID < MinoNum; ++MinoID)
	{
		if (AMino* const Mino = MinoArray[MinoID])
		{
			const FIntPoint& NewMinoLocalMatrixLocation = MinoLocalMatrixLocations[MinoID];
			Mino->SetRelativeLocationByMatrixLocation(NewMinoLocalMatrixLocation);
		}
	}
}

const FTetriminoShapeInfo& ATetrimino::GetTetriminoShapeInfoByShape(const ETetriminoShape Shape)
{
	const FTetriminoShapeInfo* TetriminoShapeInfo = TetriminoShapeInfos.Find(Shape);
	check(TetriminoShapeInfo != nullptr);
	return *TetriminoShapeInfo;
}

const FIntPoint& ATetrimino::GetInitialMatrixLocationByShape(const ETetriminoShape Shape)
{
	const FTetriminoShapeInfo& TetriminoShapeInfo = ATetrimino::GetTetriminoShapeInfoByShape(Shape);
	return TetriminoShapeInfo.InitialMatrixLocation;
}

const TArray<FIntPoint>& ATetrimino::GetSRSRotationPointOffsetsByRotationInfo(const FRotationInfo& RotationInfo)
{
	const FTetriminoShapeInfo& TetriminoShapeInfo = ATetrimino::GetTetriminoShapeInfoByShape(RotationInfo.Shape);
	return TetriminoShapeInfo.SRSRotationPointOffsetsTable[RotationInfo.Facing][RotationInfo.Direction];
}

FString ATetrimino::GetTetriminoShapeName(const ETetriminoShape Shape)
{
	static const TMap<ETetriminoShape, FString> TetriminoShapeNames =
	{
		{ETetriminoShape::O, TEXT("O")},
		{ETetriminoShape::I, TEXT("I")},
		{ETetriminoShape::T, TEXT("T")},
		{ETetriminoShape::L, TEXT("L")},
		{ETetriminoShape::J, TEXT("J")},
		{ETetriminoShape::S, TEXT("S")},
		{ETetriminoShape::Z, TEXT("Z")}
	};

	if (const FString* Name = TetriminoShapeNames.Find(Shape))
	{
		return *Name;
	}
	return TEXT("None");
}

FString ATetrimino::GetFacingName(const ETetriminoFacing Facing)
{
	static const TMap<ETetriminoFacing, FString> TetriminoFacingNames =
	{
		{ETetriminoFacing::North, TEXT("North")},
		{ETetriminoFacing::East, TEXT("East")},
		{ETetriminoFacing::South, TEXT("South")},
		{ETetriminoFacing::West, TEXT("West")},
	};

	if (const FString* Name = TetriminoFacingNames.Find(Facing))
	{
		return *Name;
	}
	return TEXT("None");
}

