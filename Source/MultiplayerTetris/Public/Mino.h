// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Mino.generated.h"

class UMaterialInterface;
class UStaticMeshComponent;

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
	static FVector Get3DRelativePositionByUnitVector2D(const FVector2D& Vector2D, const float Z = 0.0f);

	void SetRelativeLocationByUnitVector2D(const FVector2D& Vector2D);
	FVector GetRelativeLocation() const;
	void SetRelativeLocation(const FVector& NewLocation);
	void SetMaterial(UMaterialInterface* const Material, const int32 ElementIndex = 0);

public:
	static constexpr float DefaultUnitLength = 100.f;
	static constexpr float MinoScale = 0.125f;
	static constexpr float UnitLength = DefaultUnitLength * MinoScale;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MinoMesh;
};
