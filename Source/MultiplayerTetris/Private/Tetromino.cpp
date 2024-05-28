// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#include "Tetromino.h"

#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

#include "Mino.h"

const FVector2D ATetromino::DirectionLeft = FVector2D(-1, 0);
const FVector2D ATetromino::DirectionRight = FVector2D(1, 0);
const FVector2D ATetromino::DirectionDown = FVector2D(0, -1);

const TArray<ATetromino::FTetrominoInfo> ATetromino::Infos =
{
	// O
	{
		{{1, -1}, {2, -1}, {1, -2}, {2, -2}},
		"/Game/Material/M_MinoMaterial_Yellow"
	},
	// I
	{
		{{0, -1}, {1, -1}, {2, -1}, {3, -1}},
		"/Game/Material/M_MinoMaterial_Cyan"
	},
	// T
	{
		{{1, -1}, {0, -2}, {1, -2}, {2, -2}},
		"/Game/Material/M_MinoMaterial_Purple"
	},
	// L
	{
		{{0, -2}, {1, -2}, {2, -1}, {2, -2}},
		"/Game/Material/M_MinoMaterial_Orange"
	},
	// J
	{
		{{0, -1}, {0, -2}, {1, -2}, {2, -2}},
		"/Game/Material/M_MinoMaterial_Blue"
	},
	// S
	{
		{{1, -1}, {2, -1}, {0, -2}, {1, -2}},
		"/Game/Material/M_MinoMaterial_Green"
	},
	// Z
	{
		{{0, -1}, {1, -1}, {1, -2}, {2, -2}},
		"/Game/Material/M_MinoMaterial_Red"
	}
};

ATetromino::ATetromino()
	: TetrominoType(ETetrominoType::None)
	, MinoClass(AMino::StaticClass())
	, Minos()
	, GhostPiece(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and set the default root component
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

void ATetromino::SetTetrominoType(const ETetrominoType NewTetrominoType)
{
	TetrominoType = NewTetrominoType;
}

void ATetromino::Move(const FVector2D& Direction)
{
	const FVector Movement(Direction * AMino::UnitLength, 0.f);
	AddActorLocalOffset(Movement);
}

void ATetromino::InitializeMinos()
{
	const int32 TetrominoIndex = static_cast<int32>(TetrominoType);
	if (!Infos.IsValidIndex(TetrominoIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid TetrominoIndex: %d\n"), TetrominoIndex);
		return;
	}

	const FTetrominoInfo& Info = Infos[TetrominoIndex];

	UMaterialInterface* const Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *Info.MaterialPath));
	if (!Material)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load material: %s"), *Info.MaterialPath);
		return;
	}

	Minos.Reserve(Info.MinoPositions.Num());

	for (const FVector2D& MinoPosition : Info.MinoPositions)
	{
		if (AMino* const Mino = GetWorld()->SpawnActor<AMino>(MinoClass, FVector::ZeroVector, FRotator::ZeroRotator))
		{
			const int32 ElementIndex = 0;
			Mino->SetMaterial(ElementIndex, Material);

			Mino->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			const FVector MinoRelativeLocation(AMino::UnitLength * MinoPosition, 0.0f);
			Mino->SetRelativeLocation(MinoRelativeLocation);

			Minos.Add(Mino);
		}
	}
}

void ATetromino::DebugPrintState() const
{
	UE_LOG(LogTemp, Log, TEXT("TetrominoInPlay Type: %s"), *GetTetrominoTypeName(TetrominoType));

	const FVector TetrominoLocation = GetActorLocation();
	UE_LOG(LogTemp, Log, TEXT("Tetromino: Location: %s"), *TetrominoLocation.ToString());

	for (int32 Index = 0; Index < Minos.Num(); ++Index)
	{
		if (Minos[Index])
		{
			const FVector MinoRelativeLocation = Minos[Index]->GetRelativeLocation();
			UE_LOG(LogTemp, Log, TEXT("Mino %d: Relative Location: %s"), Index, *MinoRelativeLocation.ToString());
		}
	}
}

FString ATetromino::GetTetrominoTypeName(const ETetrominoType TetrominoType)
{
	static const TMap<ETetrominoType, FString> TetrominoTypeNames = {
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
