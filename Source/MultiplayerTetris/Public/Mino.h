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
	FLinearColor Color;
};

/**
 * @class UMino
 * @brief Represents a specialized static mesh component used in a multiplayer Tetris game.
 */
UCLASS()
class MULTIPLAYERTETRIS_API UMino : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UMino();

	void SetRelativeLocationByMatrixLocation(const FIntPoint& MatrixLocation, const float Z = 0.0f);
	void AttachToWithMatrixLocation(USceneComponent* const Parent, const FIntPoint& MatrixLocation, const float Z = 0.0f);

	static UMino* CreateMino(UObject* const InOuter, const FMinoInfo& MinoInfo);
	static void ClearMaterialCache();
	static bool IsMaterialCacheEmpty() { return MaterialCache.Num() == 0; }
	static FVector GetRelativeLocationByMatrixLocation(const FIntPoint& MatrixLocation, const float Z = 0.0f);
	static UMaterialInterface* GetMaterialByMinoInfo(const FMinoInfo& MinoInfo);
	static UMaterialInstanceDynamic* GetMaterialInstanceByMinoInfo(UObject* const InOuter, const FMinoInfo& MinoInfo);

private:
	static constexpr float DefaultUnitLength = 100.f;

public:
	static constexpr float MinoScale = 0.125f;
	static constexpr float UnitLength = DefaultUnitLength * MinoScale;

private:
	static const FName BaseColorParameterName;
	static const FString CubeMeshPath;
	static TMap<FString, UMaterialInstanceDynamic*> MaterialCache; // static cache for material instances
};
