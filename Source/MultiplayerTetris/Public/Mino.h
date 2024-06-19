// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

#include "Mino.generated.h"

class UMaterialInterface;

struct FMinoInfo
{
	FString MaterialPath;
	FLinearColor Color;
};

/**
 *
 */
UCLASS()
class MULTIPLAYERTETRIS_API UMino : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UMino();

	void SetRelativeLocationByMatrixLocation(const FIntPoint& MatrixLocation);

	static void ClearMaterialCache();
	static FVector Get3DRelativePositionByMatrixLocation(const FIntPoint& MatrixLocation, float Z = 0.0f);
	static UMaterialInterface* GetMaterialByMinoInfo(const FMinoInfo& MinoInfo);
	static UMaterialInstanceDynamic* GetMaterialInstanceByMinoInfo(UObject* InOuter, const FMinoInfo& MinoInfo);

public:
	static constexpr float DefaultUnitLength = 100.f;
	static constexpr float MinoScale = 0.125f;
	static constexpr float UnitLength = DefaultUnitLength * MinoScale;

private:
	static const FName BaseColorParameterName;
	static const FString CubeMeshPath;
	static TMap<FString, UMaterialInstanceDynamic*> MaterialCache; // static cache for material instances
};
