// Copyright Ryu KeunBeom. All Rights Reserved.

#include "Mino.h"

#include "UObject/ConstructorHelpers.h"

const FName UMino::BaseColorParameterName = TEXT("BaseColor");
const FName UMino::OpacityParameterName = TEXT("Opacity");
const FString UMino::CubeMeshPath = TEXT("/Engine/BasicShapes/Cube.Cube");
//const FString UMino::DefaultMaterialPath = TEXT("/Game/Material/M_MinoMaterial");
const FString UMino::DefaultMaterialPath = TEXT("/Game/Material/M_MinoOutline");


UMino::UMino()
{
	static const ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(*UMino::CubeMeshPath);
	if (CubeMesh.Succeeded())
	{
		UStaticMeshComponent::SetStaticMesh(CubeMesh.Object);
	}

	SetWorldScale3D(FVector(MinoScale));
}

void UMino::SetRelativeLocationByMatrixLocation(const FIntPoint& MatrixLocation, const float Z)
{
	SetRelativeLocation(UMino::GetRelativeLocationByMatrixLocation(MatrixLocation, Z));
}

void UMino::AttachToWithMatrixLocation(USceneComponent* const Parent, const FIntPoint& MatrixLocation, const float Z)
{
	AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);
	SetRelativeLocationByMatrixLocation(MatrixLocation, Z);
}

UMino* UMino::CreateMino(UObject* const InOuter, const FMinoInfo& MinoInfo)
{
	if (UMino* const Mino = NewObject<UMino>(InOuter))
	{
		if (UMaterialInstanceDynamic* const MaterialInstance = UMino::GetMaterialInstanceByMinoInfo(InOuter, MinoInfo))
		{
			static constexpr int32 ElementIndex = 0;
			Mino->SetMaterial(ElementIndex, MaterialInstance);
			Mino->SetTranslucentSortPriority(MinoInfo.TranslucentSortPriority);
			Mino->RegisterComponent();
			return Mino;
		}
	}
	return nullptr;
}

FVector UMino::GetRelativeLocationByMatrixLocation(const FIntPoint& MatrixLocation, const float Z)
{
	const float X = -UnitLength * MatrixLocation.Y;
	const float Y = -UnitLength * MatrixLocation.X;
	return FVector(X, Y, Z);
}

UMaterialInterface* UMino::GetMaterialByMinoInfo(const FMinoInfo& MinoInfo)
{
	UMaterialInterface* const MinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *MinoInfo.MaterialPath));
	ensureMsgf(MinoMaterial != nullptr, TEXT("Failed to load material: %s"), *MinoInfo.MaterialPath);
	return MinoMaterial;
}

UMaterialInstanceDynamic* UMino::GetMaterialInstanceByMinoInfo(UObject* const InOuter, const FMinoInfo& MinoInfo)
{
	if (UMaterialInterface* const BaseMaterial = UMino::GetMaterialByMinoInfo(MinoInfo))
	{
		if (UMaterialInstanceDynamic* const DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, InOuter))
		{
			DynamicMaterialInstance->SetVectorParameterValue(UMino::BaseColorParameterName, MinoInfo.BaseColor);
			DynamicMaterialInstance->SetScalarParameterValue(UMino::OpacityParameterName, MinoInfo.Opacity);
			return DynamicMaterialInstance;
		}
	}
	return nullptr;
}
