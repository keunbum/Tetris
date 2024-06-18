// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Mino.generated.h"

class UMaterialInterface;
class UStaticMeshComponent;

struct FMinoInfo
{
	FString MaterialPath;
	FLinearColor Color;
};

UCLASS()
class MULTIPLAYERTETRIS_API AMino : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMino();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

public:
	void SetRelativeLocationByMatrixLocation(const FIntPoint& MatrixLocation);
	FVector GetRelativeLocation() const;
	void SetRelativeLocation(const FVector& NewLocation);
	void SetMaterial(const int32 ElementIndex, UMaterialInterface* const Material);

	static FVector Get3DRelativePositionByMatrixLocation(const FIntPoint& MatrixLocation, const float Z = 0.0f);
	static UMaterialInterface* GetMaterialByMinoInfo(const FMinoInfo& MinoInfo);
	static UMaterialInstanceDynamic* GetMaterialInstanceByMinoInfo(UObject* const InOuter, const FMinoInfo& MinoInfo);
public:
	static constexpr float DefaultUnitLength = 100.f;
	static constexpr float MinoScale = 0.125f;
	static constexpr float UnitLength = DefaultUnitLength * MinoScale;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MinoMesh;
};
