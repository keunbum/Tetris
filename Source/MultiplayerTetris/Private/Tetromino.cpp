// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#include "Tetromino.h"

#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

#include "Mino.h"

const FVector2D ATetromino::MoveDirectionLeft = FVector2D(0, -1);
const FVector2D ATetromino::MoveDirectionRight = -MoveDirectionLeft;
const FVector2D ATetromino::MoveDirectionDown = MoveDirectionLeft.GetRotated(90.f);

TMap<ETetrominoType, FTetrominoInfo> ATetromino::Infos =
{
	// O
	{
		ETetrominoType::O,
		{
			{
				{ ETetrominoFacingType::North, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } },
				{ ETetrominoFacingType::East, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } },
				{ ETetrominoFacingType::South, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } },
				{ ETetrominoFacingType::West, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Yellow")
		}
	},
	// I
	{
		ETetrominoType::I,
		{
			{
				{ ETetrominoFacingType::North, { {1, 0}, {1, 1}, {1, 2}, {1, 3} } },
				{ ETetrominoFacingType::East, { {0, 2}, {1, 2}, {2, 2}, {3, 2} } },
				{ ETetrominoFacingType::South, { {2, 0}, {2, 1}, {2, 2}, {2, 3} } },
				{ ETetrominoFacingType::West, { {0, 1}, {1, 1}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Cyan")
		}
	},
	// T
	{
		ETetrominoType::T,
		{
			{
				{ ETetrominoFacingType::North, { {1, 1}, {2, 0}, {2, 1}, {2, 2} } },
				{ ETetrominoFacingType::East, { {1, 1}, {2, 1}, {2, 2}, {3, 1} } },
				{ ETetrominoFacingType::South, { {2, 0}, {2, 1}, {2, 2}, {3, 1} } },
				{ ETetrominoFacingType::West, { {1, 1}, {2, 0}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Purple")
		}
	},
	// L
	{
		ETetrominoType::L,
		{
			{
				{ ETetrominoFacingType::North, { {1, 2}, {2, 0}, {2, 1}, {2, 2} } },
				{ ETetrominoFacingType::East, { {1, 1}, {2, 1}, {3, 1}, {3, 2} } },
				{ ETetrominoFacingType::South, { {2, 0}, {2, 1}, {2, 2}, {3, 0} } },
				{ ETetrominoFacingType::West, { {1, 0}, {1, 1}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Orange")
		}
	},
	// J
	{
		ETetrominoType::J,
		{
			{
				{ ETetrominoFacingType::North, { {1, 0}, {2, 0}, {2, 1}, {2, 2} } },
				{ ETetrominoFacingType::East, { {1, 1}, {1, 2}, {2, 1}, {3, 1} } },
				{ ETetrominoFacingType::South, { {2, 0}, {2, 1}, {2, 2}, {3, 2} } },
				{ ETetrominoFacingType::West, { {1, 1}, {2, 1}, {3, 0}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Blue")
		}
	},
	// S
	{
		ETetrominoType::S,
		{
			{
				{ ETetrominoFacingType::North, { {1, 1}, {1, 2}, {2, 0}, {2, 1} } },
				{ ETetrominoFacingType::East, { {1, 1}, {2, 1}, {2, 2}, {3, 2} } },
				{ ETetrominoFacingType::South, { {2, 1}, {2, 2}, {3, 0}, {3, 1} } },
				{ ETetrominoFacingType::West, { {1, 0}, {2, 0}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Green")
		}
	},
	// Z
	{
		ETetrominoType::Z,
		{
			{
				{ ETetrominoFacingType::North, { {1, 0}, {1, 1}, {2, 1}, {2, 2} } },
				{ ETetrominoFacingType::East, { {1, 2}, {2, 1}, {2, 2}, {3, 1} } },
				{ ETetrominoFacingType::South, { {2, 0}, {2, 1}, {3, 1}, {3, 2} } },
				{ ETetrominoFacingType::West, { {1, 1}, {2, 0}, {2, 1}, {3, 0} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Red")
		}
	}
};



ATetromino::ATetromino()
	: TetrominoType(ETetrominoType::None)
	, FacingType(ETetrominoFacingType::North)
	, MinoClass(AMino::StaticClass())
	, Minos()
	, bIsGhostPieceOn(true)
	, GhostPiece(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void ATetromino::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATetromino::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATetromino::Initialize()
{
	InitializeMinos();
}

void ATetromino::Move(const FVector2D& Direction)
{
	const FVector DeltaLocation(AMino::Get3DRelativePositionByUnitVector2D(Direction));
	AddActorLocalOffset(DeltaLocation);
	//DebugPrintState();
}

void ATetromino::RotateTo(const int32 Direction)
{
	SetFacingType(FacingType + Direction);
	UpdateMinoPositions();
}

void ATetromino::InitializeMinos()
{
	FTetrominoInfo Info;
	if (!GetTetrominoInfo(Info, TetrominoType))
	{
		return;
	}

	UMaterialInterface* const MinoMaterial = GetMaterial(Info);
	if (!MinoMaterial)
	{
		return;
	}

	const TArray<FVector2D>& MinoUnitPositions = Info.GetMinoUnitPosition(FacingType);
	const int32 MinoNum = MinoUnitPositions.Num();
	check(MinoNum == 4);
	Minos.Reserve(MinoNum);

	for (const FVector2D& MinoUnitPosition : MinoUnitPositions)
	{
		if (AMino* const Mino = GetWorld()->SpawnActor<AMino>(MinoClass, FVector::ZeroVector, FRotator::ZeroRotator))
		{
			Mino->SetMaterial(MinoMaterial);

			Mino->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			Mino->SetRelativeLocationByUnitVector2D(MinoUnitPosition);

			Minos.Add(Mino);
		}
	}
}

void ATetromino::UpdateMinoPositions()
{
	FTetrominoInfo Info;
	if (!GetTetrominoInfo(Info, TetrominoType))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("FacingType: %d, %s"), FacingType + 0, *GetFacingTypeName(FacingType));
	check(0 <= static_cast<int32>(FacingType) && static_cast<int32>(FacingType) <= Info.MinoUnitPositionsByFacing.Num());

	const TArray<FVector2D>& MinoUnitPositions = Info.GetMinoUnitPosition(FacingType);
	for (int32 MinoID = 0; MinoID < Minos.Num(); ++MinoID)
	{
		if (AMino* const Mino = Minos[MinoID])
		{
			check(MinoUnitPositions.IsValidIndex(MinoID));
			const FVector2D& UnitVector2D = MinoUnitPositions[MinoID];
			Mino->SetRelativeLocationByUnitVector2D(UnitVector2D);
		}
	}
}

bool ATetromino::GetTetrominoInfo(FTetrominoInfo& OutInfo, const ETetrominoType TetrominoType)
{
	if (const FTetrominoInfo* Info =  Infos.Find(TetrominoType))
	{
		OutInfo = *Info;
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("Invalid TetrominoType: %s\n"), *GetTetrominoTypeName(TetrominoType));
	return false;
}

UMaterialInterface* ATetromino::GetMaterial(const FTetrominoInfo& Info)
{
	UMaterialInterface* const MinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *Info.MaterialPath));
	if (!MinoMaterial)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load material: %s"), *Info.MaterialPath);
	}
	return MinoMaterial;
}

FString ATetromino::GetTetrominoTypeName(const ETetrominoType TetrominoType)
{
	static const TMap<ETetrominoType, FString> TetrominoTypeNames =
	{
		{ETetrominoType::O, TEXT("O")},
		{ETetrominoType::I, TEXT("I")},
		{ETetrominoType::L, TEXT("L")},
		{ETetrominoType::J, TEXT("J")},
		{ETetrominoType::T, TEXT("T")},
		{ETetrominoType::S, TEXT("S")},
		{ETetrominoType::Z, TEXT("Z")}
	};

	if (const FString* Name = TetrominoTypeNames.Find(TetrominoType))
	{
		return *Name;
	}
	return TEXT("None");
}

FString ATetromino::GetFacingTypeName(const ETetrominoFacingType FacingType)
{
	static const TMap<ETetrominoFacingType, FString> TetrominoFacingTypeNames =
	{
		{ETetrominoFacingType::North, TEXT("North")},
		{ETetrominoFacingType::East, TEXT("East")},
		{ETetrominoFacingType::South, TEXT("South")},
		{ETetrominoFacingType::West, TEXT("West")},
	};

	if (const FString* Name = TetrominoFacingTypeNames.Find(FacingType))
	{
		return *Name;
	}
	return TEXT("None");
}

void ATetromino::DebugPrintState() const
{
	UE_LOG(LogTemp, Log, TEXT("TetrominoInPlay Type: %s"), *GetTetrominoTypeName(TetrominoType));
	UE_LOG(LogTemp, Log, TEXT("Tetromino: Location: %s"), *GetActorLocation().ToString());

	for (int32 Index = 0; Index < Minos.Num(); ++Index)
	{
		if (Minos[Index])
		{
			const FVector MinoRelativeLocation = Minos[Index]->GetRelativeLocation();
			UE_LOG(LogTemp, Log, TEXT("Mino %d: Relative Location: %s"), Index, *MinoRelativeLocation.ToString());
		}
	}
}
