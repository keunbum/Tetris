// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#include "Tetrimino.h"

#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

#include "Mino.h"

const FVector2D ATetrimino::MoveDirectionLeft = FVector2D(0, -1);
const FVector2D ATetrimino::MoveDirectionRight = -MoveDirectionLeft;
const FVector2D ATetrimino::MoveDirectionDown = MoveDirectionLeft.GetRotated(90.f);

const TMap<ETetriminoShape, FTetriminoInfo> ATetrimino::TetriminoInfos =
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
			TEXT("/Game/Material/M_MinoMaterial_Yellow")
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
			TEXT("/Game/Material/M_MinoMaterial_Cyan")
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
			TEXT("/Game/Material/M_MinoMaterial_Purple")
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
			TEXT("/Game/Material/M_MinoMaterial_Orange")
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
			TEXT("/Game/Material/M_MinoMaterial_Blue")
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
			TEXT("/Game/Material/M_MinoMaterial_Green")
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
			TEXT("/Game/Material/M_MinoMaterial_Red")
		}
	}
};

ATetrimino::ATetrimino()
	: TetriminoShape(ETetriminoShape::None)
	, Facing(ETetriminoFacing::North)
	, MinoClass(AMino::StaticClass())
	, Minos()
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

void ATetrimino::Initialize(const ETetriminoShape NewTetriminoShape)
{
	SetTetriminoShape(NewTetriminoShape);
	InitializeMinos();
}

void ATetrimino::MoveBy(const FVector2D& Vector2D)
{
	const FVector DeltaLocation(AMino::Get3DRelativePositionByUnitVector2D(Vector2D));
	AddActorLocalOffset(DeltaLocation);
	//DebugPrintState();
}

void ATetrimino::RotateBy(const int32 Value)
{
	SetFacing(Facing + Value);
	UpdateMinoPositions();
}

void ATetrimino::InitializeMinos()
{
	FTetriminoInfo TetriminoInfo;
	if (!GetTetriminoInfoByShape(TetriminoInfo, TetriminoShape))
	{
		return;
	}

	UMaterialInterface* const MinoMaterial = GetMaterialByTetriminoInfo(TetriminoInfo);
	if (!MinoMaterial)
	{
		return;
	}

	const TArray<FVector2D>& MinoUnitPositions = TetriminoInfo.GetMinoUnitPosition(Facing);
	check(MinoUnitPositions.Num() == MinoNum);

	for (int32 MinoID = 0; MinoID < MinoNum; ++MinoID)
	{
		const FVector2D& MinoUnitPosition = MinoUnitPositions[MinoID];
		if (AMino* const Mino = GetWorld()->SpawnActor<AMino>(MinoClass, FVector::ZeroVector, FRotator::ZeroRotator))
		{
			Mino->SetMaterial(MinoMaterial);

			Mino->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			Mino->SetRelativeLocationByUnitVector2D(MinoUnitPosition);

			Minos[MinoID] = Mino;
		}
	}
}

void ATetrimino::UpdateMinoPositions()
{
	FTetriminoInfo TetriminoInfo;
	if (!GetTetriminoInfoByShape(TetriminoInfo, TetriminoShape))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Facing: %d, %s"), static_cast<int32>(Facing), *GetFacingName(Facing));
	check(0 <= static_cast<int32>(Facing) && static_cast<int32>(Facing) <= TetriminoInfo.MinoUnitPositionsByFacing.Num());

	const TArray<FVector2D>& MinoUnitPositions = TetriminoInfo.GetMinoUnitPosition(Facing);
	for (int32 MinoID = 0; MinoID < MinoNum; ++MinoID)
	{
		if (AMino* const Mino = Minos[MinoID])
		{
			check(MinoUnitPositions.IsValidIndex(MinoID));
			const FVector2D& UnitVector2D = MinoUnitPositions[MinoID];
			Mino->SetRelativeLocationByUnitVector2D(UnitVector2D);
		}
	}
}

void ATetrimino::DebugPrintState() const
{
	UE_LOG(LogTemp, Log, TEXT("TetriminoInPlay Type: %s"), *GetTetriminoShapeName(TetriminoShape));
	UE_LOG(LogTemp, Log, TEXT("Tetromino: Location: %s"), *GetActorLocation().ToString());

	for (int32 Index = 0; Index < MinoNum; ++Index)
	{
		if (Minos[Index])
		{
			const FVector MinoRelativeLocation = Minos[Index]->GetRelativeLocation();
			UE_LOG(LogTemp, Log, TEXT("Mino %d: Relative Location: %s"), Index, *MinoRelativeLocation.ToString());
		}
	}
}

bool ATetrimino::GetTetriminoInfoByShape(FTetriminoInfo& OutTetriminoInfo, const ETetriminoShape TetriminoShape)
{
	if (const FTetriminoInfo* TetriminoInfo = TetriminoInfos.Find(TetriminoShape))
	{
		OutTetriminoInfo = *TetriminoInfo;
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("Invalid TetriminoShape: %s\n"), *GetTetriminoShapeName(TetriminoShape));
	return false;
}

UMaterialInterface* ATetrimino::GetMaterialByTetriminoInfo(const FTetriminoInfo& TetriminoInfo)
{
	UMaterialInterface* const MinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *TetriminoInfo.MaterialPath));
	if (!MinoMaterial)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load material: %s"), *TetriminoInfo.MaterialPath);
	}
	return MinoMaterial;
}

FString ATetrimino::GetTetriminoShapeName(const ETetriminoShape TetriminoShape)
{
	static const TMap<ETetriminoShape, FString> TetrominoTypeNames =
	{
		{ETetriminoShape::O, TEXT("O")},
		{ETetriminoShape::I, TEXT("I")},
		{ETetriminoShape::T, TEXT("T")},
		{ETetriminoShape::L, TEXT("L")},
		{ETetriminoShape::J, TEXT("J")},
		{ETetriminoShape::S, TEXT("S")},
		{ETetriminoShape::Z, TEXT("Z")}
	};

	if (const FString* Name = TetrominoTypeNames.Find(TetriminoShape))
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

ETetriminoShape ATetrimino::GetTetriminoShapeRandom()
{
	const int32 RandomIndex = FMath::RandRange(0, static_cast<int32>(ETetriminoShape::Max) - 1);
	const ETetriminoShape TetriminoShape = static_cast<ETetriminoShape>(RandomIndex);
	return TetriminoShape;
}

