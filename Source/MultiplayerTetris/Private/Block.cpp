// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "Block.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

TMap<FString, UMaterialInstanceDynamic*> ABlock::MaterialCache;

// Sets default values
ABlock::ABlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MinoMesh"));
	check(MinoMesh != nullptr);
	RootComponent = MinoMesh;

	// 기본 큐브 메시 사용
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		MinoMesh->SetStaticMesh(CubeMesh.Object);
	}

	MinoMesh->SetWorldScale3D(FVector(MinoScale));
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABlock::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlock::SetRelativeLocationByMatrixLocation(const FIntPoint& MatrixLocation)
{
	const FVector RelativeLocation(ABlock::Get3DRelativePositionByMatrixLocation(MatrixLocation));
	SetRelativeLocation(RelativeLocation);
}

FVector ABlock::GetRelativeLocation() const
{
	return MinoMesh->GetRelativeLocation();
}

void ABlock::SetRelativeLocation(const FVector& NewLocation)
{
	MinoMesh->SetRelativeLocation(NewLocation);
}

void ABlock::SetMaterial(const int32 ElementIndex, UMaterialInterface* const Material)
{
	MinoMesh->SetMaterial(ElementIndex, Material);
}

void ABlock::ClearMaterialCache()
{
	MaterialCache.Empty();
}

FVector ABlock::Get3DRelativePositionByMatrixLocation(const FIntPoint& MatrixLocation, const float Z)
{
	const float X = -UnitLength * MatrixLocation.Y;
	const float Y = -UnitLength * MatrixLocation.X;
	return FVector(X, Y, Z);
}

UMaterialInterface* ABlock::GetMaterialByMinoInfo(const FBlockInfo& MinoInfo)
{
	UMaterialInterface* const MinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *MinoInfo.MaterialPath));
	ensureMsgf(MinoMaterial != nullptr, TEXT("Failed to load material: %s"), *MinoInfo.MaterialPath);
	return MinoMaterial;
}

UMaterialInstanceDynamic* ABlock::GetMaterialInstanceByMinoInfo(UObject* const InOuter, const FBlockInfo& MinoInfo)
{
	// Create a unique key combining material path and color
	const FString MaterialKey = MinoInfo.MaterialPath + MinoInfo.Color.ToString();

	// Check if the material instance already exists in the cache
	if (UMaterialInstanceDynamic** const FoundMaterial = MaterialCache.Find(MaterialKey))
	{
		return *FoundMaterial;
	}

	// If not found, create a new material instance
	if (UMaterialInterface* const BaseMaterial = ABlock::GetMaterialByMinoInfo(MinoInfo))
	{
		if (UMaterialInstanceDynamic* const DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, InOuter))
		{
			DynamicMaterialInstance->SetVectorParameterValue("BaseColor", MinoInfo.Color);
			MaterialCache.Add(MaterialKey, DynamicMaterialInstance);
			return DynamicMaterialInstance;
		}
	}
	return nullptr;
}