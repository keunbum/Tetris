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

FVector AMino::Get3DRelativePositionByIntVector2D(const FIntVector2& IntVector2D, const float Z)
{
	const float X = -UnitLength * IntVector2D.Y;
	const float Y = -UnitLength * IntVector2D.X;
	return FVector(X, Y, Z);
}

void AMino::SetRelativeLocationByIntVector2D(const FIntVector2& IntVector2D)
{
	const FVector RelativeLocation(AMino::Get3DRelativePositionByIntVector2D(IntVector2D));
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

void AMino::SetMaterial(UMaterialInterface* const Material, const int32 ElementIndex)
{
	MinoMesh->SetMaterial(ElementIndex, Material);
}
