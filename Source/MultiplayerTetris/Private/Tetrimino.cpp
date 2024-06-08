// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#include "Tetrimino.h"

#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

#include "Mino.h"

const FVector2D ATetrimino::MoveDirectionLeft = FVector2D(0, -1);
const FVector2D ATetrimino::MoveDirectionRight = -MoveDirectionLeft;
const FVector2D ATetrimino::MoveDirectionDown = MoveDirectionLeft.GetRotated(90.f);

const TMap<ETetriminoType, FTetrominoInfo> ATetrimino::TetrominoInfos =
{
	{
		ETetriminoType::O,
		{
			{
				{ ETetriminoFacingType::North, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } },
				{ ETetriminoFacingType::East, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } },
				{ ETetriminoFacingType::South, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } },
				{ ETetriminoFacingType::West, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Yellow")
		}
	},
	{
		ETetriminoType::I,
		{
			{
				{ ETetriminoFacingType::North, { {1, 0}, {1, 1}, {1, 2}, {1, 3} } },
				{ ETetriminoFacingType::East, { {0, 2}, {1, 2}, {2, 2}, {3, 2} } },
				{ ETetriminoFacingType::South, { {2, 0}, {2, 1}, {2, 2}, {2, 3} } },
				{ ETetriminoFacingType::West, { {0, 1}, {1, 1}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Cyan")
		}
	},
	{
		ETetriminoType::T,
		{
			{
				{ ETetriminoFacingType::North, { {1, 1}, {2, 0}, {2, 1}, {2, 2} } },
				{ ETetriminoFacingType::East, { {1, 1}, {2, 1}, {2, 2}, {3, 1} } },
				{ ETetriminoFacingType::South, { {2, 0}, {2, 1}, {2, 2}, {3, 1} } },
				{ ETetriminoFacingType::West, { {1, 1}, {2, 0}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Purple")
		}
	},
	{
		ETetriminoType::L,
		{
			{
				{ ETetriminoFacingType::North, { {1, 2}, {2, 0}, {2, 1}, {2, 2} } },
				{ ETetriminoFacingType::East, { {1, 1}, {2, 1}, {3, 1}, {3, 2} } },
				{ ETetriminoFacingType::South, { {2, 0}, {2, 1}, {2, 2}, {3, 0} } },
				{ ETetriminoFacingType::West, { {1, 0}, {1, 1}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Orange")
		}
	},
	{
		ETetriminoType::J,
		{
			{
				{ ETetriminoFacingType::North, { {1, 0}, {2, 0}, {2, 1}, {2, 2} } },
				{ ETetriminoFacingType::East, { {1, 1}, {1, 2}, {2, 1}, {3, 1} } },
				{ ETetriminoFacingType::South, { {2, 0}, {2, 1}, {2, 2}, {3, 2} } },
				{ ETetriminoFacingType::West, { {1, 1}, {2, 1}, {3, 0}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Blue")
		}
	},
	{
		ETetriminoType::S,
		{
			{
				{ ETetriminoFacingType::North, { {1, 1}, {1, 2}, {2, 0}, {2, 1} } },
				{ ETetriminoFacingType::East, { {1, 1}, {2, 1}, {2, 2}, {3, 2} } },
				{ ETetriminoFacingType::South, { {2, 1}, {2, 2}, {3, 0}, {3, 1} } },
				{ ETetriminoFacingType::West, { {1, 0}, {2, 0}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Green")
		}
	},
	{
		ETetriminoType::Z,
		{
			{
				{ ETetriminoFacingType::North, { {1, 0}, {1, 1}, {2, 1}, {2, 2} } },
				{ ETetriminoFacingType::East, { {1, 2}, {2, 1}, {2, 2}, {3, 1} } },
				{ ETetriminoFacingType::South, { {2, 0}, {2, 1}, {3, 1}, {3, 2} } },
				{ ETetriminoFacingType::West, { {1, 1}, {2, 0}, {2, 1}, {3, 0} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Red")
		}
	}
};

ATetrimino::ATetrimino()
	: TetriminoType(ETetriminoType::None)
	, FacingType(ETetriminoFacingType::North)
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

void ATetrimino::Initialize(const ETetriminoType NewTetrominoType)
{
	SetTetriminoType(NewTetrominoType);
	InitializeMinos();
}

void ATetrimino::Move(const FVector2D& Direction2D)
{
	const FVector DeltaLocation(AMino::Get3DRelativePositionByUnitVector2D(Direction2D));
	AddActorLocalOffset(DeltaLocation);
	//DebugPrintState();
}

void ATetrimino::RotateTo(const int32 Direction)
{
	SetFacingType(FacingType + Direction);
	UpdateMinoPositions();
}

void ATetrimino::InitializeMinos()
{
	FTetrominoInfo Info;
	if (!GetTetrominoInfo(Info, TetriminoType))
	{
		return;
	}

	UMaterialInterface* const MinoMaterial = GetMaterialByTetrominoInfo(Info);
	if (!MinoMaterial)
	{
		return;
	}

	const TArray<FVector2D>& MinoUnitPositions = Info.GetMinoUnitPosition(FacingType);
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
	FTetrominoInfo Info;
	if (!GetTetrominoInfo(Info, TetriminoType))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("FacingType: %d, %s"), static_cast<int32>(FacingType), *GetFacingTypeName(FacingType));
	check(0 <= static_cast<int32>(FacingType) && static_cast<int32>(FacingType) <= Info.MinoUnitPositionsByFacing.Num());

	const TArray<FVector2D>& MinoUnitPositions = Info.GetMinoUnitPosition(FacingType);
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
	UE_LOG(LogTemp, Log, TEXT("TetrominoInPlay Type: %s"), *GetTetrominoTypeName(TetriminoType));
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

bool ATetrimino::GetTetrominoInfo(FTetrominoInfo& OutTetrominoInfo, const ETetriminoType TetriminoType)
{
	if (const FTetrominoInfo* TetrominoInfo = TetrominoInfos.Find(TetriminoType))
	{
		OutTetrominoInfo = *TetrominoInfo;
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("Invalid TetriminoType: %s\n"), *GetTetrominoTypeName(TetriminoType));
	return false;
}

UMaterialInterface* ATetrimino::GetMaterialByTetrominoInfo(const FTetrominoInfo& TetrominoInfo)
{
	UMaterialInterface* const MinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *TetrominoInfo.MaterialPath));
	if (!MinoMaterial)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load material: %s"), *TetrominoInfo.MaterialPath);
	}
	return MinoMaterial;
}

FString ATetrimino::GetTetrominoTypeName(const ETetriminoType TetriminoType)
{
	static const TMap<ETetriminoType, FString> TetrominoTypeNames =
	{
		{ETetriminoType::O, TEXT("O")},
		{ETetriminoType::I, TEXT("I")},
		{ETetriminoType::T, TEXT("T")},
		{ETetriminoType::L, TEXT("L")},
		{ETetriminoType::J, TEXT("J")},
		{ETetriminoType::S, TEXT("S")},
		{ETetriminoType::Z, TEXT("Z")}
	};

	if (const FString* Name = TetrominoTypeNames.Find(TetriminoType))
	{
		return *Name;
	}
	return TEXT("None");
}

FString ATetrimino::GetFacingTypeName(const ETetriminoFacingType FacingType)
{
	static const TMap<ETetriminoFacingType, FString> TetrominoFacingTypeNames =
	{
		{ETetriminoFacingType::North, TEXT("North")},
		{ETetriminoFacingType::East, TEXT("East")},
		{ETetriminoFacingType::South, TEXT("South")},
		{ETetriminoFacingType::West, TEXT("West")},
	};

	if (const FString* Name = TetrominoFacingTypeNames.Find(FacingType))
	{
		return *Name;
	}
	return TEXT("None");
}

ETetriminoType ATetrimino::GetTetriminoTypeRandom()
{
	const int32 RandomIndex = FMath::RandRange(0, static_cast<int32>(ETetriminoType::Max) - 1);
	const ETetriminoType TetriminoType = static_cast<ETetriminoType>(RandomIndex);
	return TetriminoType;
}

