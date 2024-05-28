// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "Mino.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AMino::AMino()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MinoMesh"));
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

FVector AMino::GetUnitLengthVectorByVector2D(const FVector2D& Vector2D)
{
	static constexpr float Z = 0.0f;
	return FVector(UnitLength * Vector2D, Z);
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
