// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "Mino.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

TMap<FString, UMaterialInstanceDynamic*> AMino::MaterialCache;

// Sets default values
AMino::AMino()
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
void AMino::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMino::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMino::SetRelativeLocationByMatrixLocation(const FIntPoint& MatrixLocation)
{
	const FVector RelativeLocation(AMino::Get3DRelativePositionByMatrixLocation(MatrixLocation));
	SetRelativeLocation(RelativeLocation);
}

FVector AMino::GetRelativeLocation() const
{
	return MinoMesh->GetRelativeLocation();
}

void AMino::SetRelativeLocation(const FVector& NewLocation)
{
	MinoMesh->SetRelativeLocation(NewLocation);
}

void AMino::SetMaterial(const int32 ElementIndex, UMaterialInterface* const Material)
{
	MinoMesh->SetMaterial(ElementIndex, Material);
}

void AMino::ClearMaterialCache()
{
	MaterialCache.Empty();
}

FVector AMino::Get3DRelativePositionByMatrixLocation(const FIntPoint& MatrixLocation, const float Z)
{
	const float X = -UnitLength * MatrixLocation.Y;
	const float Y = -UnitLength * MatrixLocation.X;
	return FVector(X, Y, Z);
}

UMaterialInterface* AMino::GetMaterialByMinoInfo(const FMinoInfo& MinoInfo)
{
	UMaterialInterface* const MinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *MinoInfo.MaterialPath));
	ensureMsgf(MinoMaterial != nullptr, TEXT("Failed to load material: %s"), *MinoInfo.MaterialPath);
	return MinoMaterial;
}

UMaterialInstanceDynamic* AMino::GetMaterialInstanceByMinoInfo(UObject* const InOuter, const FMinoInfo& MinoInfo)
{
	// Create a unique key combining material path and color
	const FString MaterialKey = MinoInfo.MaterialPath + MinoInfo.Color.ToString();

	// Check if the material instance already exists in the cache
	if (UMaterialInstanceDynamic** const FoundMaterial = MaterialCache.Find(MaterialKey))
	{
		return *FoundMaterial;
	}

	// If not found, create a new material instance
	if (UMaterialInterface* const BaseMaterial = GetMaterialByMinoInfo(MinoInfo))
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