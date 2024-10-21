// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetriminoBase.h"

#include "Mino.h"
#include "Board.h"

// (행, 열) 기반으로 방향을 나타냄. (0행 0열이 기준)
const FVector2D ATetriminoBase::MoveDirectionLeft = FVector2D(0, -1);
const FVector2D ATetriminoBase::MoveDirectionRight = -MoveDirectionLeft;
const FVector2D ATetriminoBase::MoveDirectionDown = MoveDirectionLeft.GetRotated(90.f);

/* 
 * 테트리미노 모양별 정보
 * 모두 (행, 열) 기반으로 표현됨.
 * 예를 들어 T의 North가 {1, 1}, {2, 0}, {2, 1}, {2, 2}와 같이 표현되는 건 각각 1행 1열, 2행 0열, 2행 1열, 2행 2열에 해당하는 미노들이라는 뜻.
 * 그림으로 나타내면 아래와 같다.
 * x x x x
 * x o x x
 * o o o x
 * x x x x
 */
const TMap<ETetriminoShape, FTetriminoShapeInfo> ATetriminoBase::TetriminoShapeInfos =
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
			FLinearColor::Yellow,
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 3, ABoard::TetriminoDefaultSpawnLocationY),
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
			FLinearColor(0.0f, 1.0f, 1.0f),
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY),
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
			FLinearColor(0.5f, 0.0f, 0.5f),
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 3, ABoard::TetriminoDefaultSpawnLocationY),
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
			FLinearColor(1.0f, 0.5f, 0.0f),
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 3, ABoard::TetriminoDefaultSpawnLocationY),
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
			FLinearColor::Blue,
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 3, ABoard::TetriminoDefaultSpawnLocationY),
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
			FLinearColor::Green,
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 3, ABoard::TetriminoDefaultSpawnLocationY),
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
			FLinearColor::Red,
			FIntPoint(ABoard::TetriminoDefaultSpawnLocationX - 3, ABoard::TetriminoDefaultSpawnLocationY),
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

ATetriminoBase::ATetriminoBase()
	: Shape(ETetriminoShape::None)
	, Facing(ETetriminoFacing::None)
	, MatrixLocation(FIntPoint(0, 0))
	, MinoClass(UMino::StaticClass())
	, MinoArray()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

const TArray<FIntPoint>& ATetriminoBase::GetMinoTetriminoLocalLocations() const
{
	return ATetriminoBase::GetMinoTetriminoLocalLocationsByTetriminoShapeAndFacing(Shape, Facing);
}

const TArray<FIntPoint>& ATetriminoBase::GetSRSRotationPointOffsets(const ETetriminoRotationDirection RotationDirection) const
{
	const FRotationInfo RotationInfo(Shape, Facing, RotationDirection);
	return ATetriminoBase::GetSRSRotationPointOffsetsByRotationInfo(RotationInfo);
}

const FTetriminoShapeInfo& ATetriminoBase::GetTetriminoShapeInfo() const
{
	return ATetriminoBase::GetTetriminoShapeInfoByShape(Shape);
}

const FIntPoint& ATetriminoBase::GetInitialMatrixLocation() const
{
	return ATetriminoBase::GetInitialMatrixLocationByShape(Shape);
}

void ATetriminoBase::SetRelativeLocationByMatrixLocation(const FIntPoint& NewMatrixLocation)
{
	const FVector ActorLocalOffset(UMino::GetRelativeLocationByMatrixLocation(NewMatrixLocation));
	SetActorRelativeLocation(ActorLocalOffset);
	SetMatrixLocation(NewMatrixLocation);
}

void ATetriminoBase::Initialize(const FInitializeParams& Params)
{
	SetShape(Params.Shape);
	SetFacing(Params.Facing);

	InitializeMinoArray();
}

void ATetriminoBase::UpdateMinoTetriminoLocalLocations()
{
	const TArray<FIntPoint>& MinoTetriminoLocalLocations = GetMinoTetriminoLocalLocations();
	for (int32 MinoIndex = 0; MinoIndex < MinoNum; ++MinoIndex)
	{
		if (UMino* const Mino = MinoArray[MinoIndex])
		{
			const FIntPoint& NewMinoTetriminoLocalLocation = MinoTetriminoLocalLocations[MinoIndex];
			Mino->SetRelativeLocationByMatrixLocation(NewMinoTetriminoLocalLocation);
		}
	}
}

void ATetriminoBase::DetachMinos()
{
	for (UMino* const Mino : MinoArray)
	{
		if (Mino)
		{
			Mino->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		}
	}
}

void ATetriminoBase::RotateByFacing(const ETetriminoFacing NewFacing)
{
	SetFacing(NewFacing);
	UpdateMinoTetriminoLocalLocations();
}

void ATetriminoBase::DebugPrintState() const
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

ETetriminoShape ATetriminoBase::GetTetriminoShapeRandom()
{
	const int32 RandomIndex = FMath::RandRange(0, static_cast<int32>(ETetriminoShape::Max) - 1);
	const ETetriminoShape TetriminoShape = static_cast<ETetriminoShape>(RandomIndex);
	return TetriminoShape;
}

const TArray<FIntPoint>& ATetriminoBase::GetMinoTetriminoLocalLocationsByTetriminoShapeAndFacing(const ETetriminoShape Shape, const ETetriminoFacing Facing)
{
	const FTetriminoShapeInfo& TetriminoShapeInfo = ATetriminoBase::GetTetriminoShapeInfoByShape(Shape);
	return TetriminoShapeInfo.MinoTetriminoLocalLocationsByFacing[Facing];
}

FIntPoint ATetriminoBase::GetMatrixMovementIntPointByDirection(const FVector2D& Direction)
{
	static constexpr float OneSpace = 1.0f;
	const FVector2D MovementVector2D = OneSpace * Direction;
	return FIntPoint(static_cast<int32>(MovementVector2D.X), static_cast<int32>(MovementVector2D.Y));
}

void ATetriminoBase::InitializeMinoArray()
{
	const TArray<FIntPoint>& MinoTetriminoLocalLocations = GetMinoTetriminoLocalLocations();
	const FMinoInfo MinoInfo = GetMinoInfo();

	DestroyMinos();
	MinoArray.Empty(MinoNum);

	for (const FIntPoint& MinoTetriminoLocalLocation : MinoTetriminoLocalLocations)
	{
		if (UMino* const Mino = UMino::NewMino(this, MinoInfo))
		{
			Mino->AttachToWithMatrixLocation(RootComponent, MinoTetriminoLocalLocation);
			MinoArray.Add(Mino);
		}
	}
}

void ATetriminoBase::DestroyMinos()
{
	for (UMino* const Mino : MinoArray)
	{
		if (Mino)
		{
			Mino->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			Mino->DestroyComponent();
		}
	}
}

void ATetriminoBase::AddRelativeLocationByMatrixLocationOffset(const FIntPoint& MatrixLocationOffset)
{
	const FVector ActorLocalOffset(UMino::GetRelativeLocationByMatrixLocation(MatrixLocationOffset));
	AddActorLocalOffset(ActorLocalOffset);
	MatrixLocation += MatrixLocationOffset;
}

void ATetriminoBase::AttachToComponentByMatrixLocation(USceneComponent* const NewParentComponent, const FIntPoint& InitialMatrixLocation)
{
	if (RootComponent)
	{
		RootComponent->AttachToComponent(NewParentComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SetRelativeLocationByMatrixLocation(InitialMatrixLocation);
	}
}

const FTetriminoShapeInfo& ATetriminoBase::GetTetriminoShapeInfoByShape(const ETetriminoShape Shape)
{
	const FTetriminoShapeInfo* TetriminoShapeInfo = TetriminoShapeInfos.Find(Shape);
	check(TetriminoShapeInfo != nullptr);
	return *TetriminoShapeInfo;
}

const FIntPoint& ATetriminoBase::GetInitialMatrixLocationByShape(const ETetriminoShape Shape)
{
	const FTetriminoShapeInfo& TetriminoShapeInfo = ATetriminoBase::GetTetriminoShapeInfoByShape(Shape);
	return TetriminoShapeInfo.InitialMatrixLocation;
}

const TArray<FIntPoint>& ATetriminoBase::GetSRSRotationPointOffsetsByRotationInfo(const FRotationInfo& RotationInfo)
{
	const FTetriminoShapeInfo& TetriminoShapeInfo = ATetriminoBase::GetTetriminoShapeInfoByShape(RotationInfo.Shape);
	return TetriminoShapeInfo.SRSRotationPointOffsetsTable[RotationInfo.Facing][RotationInfo.Direction];
}

FString ATetriminoBase::GetTetriminoShapeName(const ETetriminoShape Shape)
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

FString ATetriminoBase::GetFacingName(const ETetriminoFacing Facing)
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

