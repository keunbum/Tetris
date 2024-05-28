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

	// 기본 큐브 메시를 사용합니다. 필요한 경우 이를 변경할 수 있습니다.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		MinoMesh->SetStaticMesh(CubeMesh.Object);
	}

	// 크기를 테트리스 블록 크기에 맞게 조정합니다.
	MinoMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
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

FVector AMino::GetRelativeLocation() const
{
	return MinoMesh->GetRelativeLocation();
}

void AMino::SetRelativeLocation(const FVector NewLocation)
{
	MinoMesh->SetRelativeLocation(NewLocation);
}

void AMino::SetMaterial(const int32 ElementIndex, UMaterialInterface* const Material)
{
	MinoMesh->SetMaterial(ElementIndex, Material);
}
