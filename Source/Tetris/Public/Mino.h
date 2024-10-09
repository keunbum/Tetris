// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

#include "Mino.generated.h"

class UMaterialInterface;

/**
 * @struct FMinoInfo
 * @brief Represents information about a "Mino" object.
 */
struct FMinoInfo
{
	FString MaterialPath;
	FLinearColor BaseColor;
	float Opacity;
	int32 TranslucentSortPriority;

	FMinoInfo()
		: FMinoInfo(TEXT(""), FLinearColor::White, 1.0f, 0)
	{
	}

	FMinoInfo(const FString& InMaterialPath, const FLinearColor& InBaseColor, const float InOpacity, const int32 InTranslucentSortPriority)
		: MaterialPath(InMaterialPath)
		, BaseColor(InBaseColor)
		, Opacity(InOpacity)
		, TranslucentSortPriority(InTranslucentSortPriority)
	{
	}
};

/**
 * @class UMino
 * @brief Represents a specialized static mesh component used in a Tetris.
 */
UCLASS()
class TETRIS_API UMino : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UMino();

	void SetRelativeLocationByMatrixLocation(const FIntPoint& MatrixLocation, const float Z = 0.0f);
	void AttachToWithMatrixLocation(USceneComponent* const Parent, const FIntPoint& MatrixLocation, const float Z = 0.0f);

	static UMino* CreateMino(UObject* const InOuter, const FMinoInfo& MinoInfo);
	static FVector GetRelativeLocationByMatrixLocation(const FIntPoint& MatrixLocation, const float Z = 0.0f);
	static UMaterialInterface* GetMaterialByMinoInfo(const FMinoInfo& MinoInfo);
	static UMaterialInstanceDynamic* GetMaterialInstanceByMinoInfo(UObject* const InOuter, const FMinoInfo& MinoInfo);

private:
	static constexpr float DefaultUnitLength = 100.f;

public:
	static constexpr float MinoScale = 1.0f;
	static constexpr float UnitLength = DefaultUnitLength * MinoScale;
	static const FString DefaultMaterialPath;

private:
	static const FName BaseColorParameterName;
	static const FName OpacityParameterName;
	static const FString CubeMeshPath;
};
