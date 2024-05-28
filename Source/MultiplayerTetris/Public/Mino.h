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
	FVector GetRelativeLocation() const;
	void SetRelativeLocation(const FVector NewLocation);
	void SetMaterial(const int32 ElementIndex, UMaterialInterface* const Material);

public:
	static constexpr float UnitLength = 50.f;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MinoMesh;
};
