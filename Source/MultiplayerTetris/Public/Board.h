// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Tetrimino.h"

#include "Board.generated.h"

class AMino;

UCLASS()
class MULTIPLAYERTETRIS_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

public:
	void AttachTetromino(ATetrimino* const NewTetromino);

private:
	void Initialize();
	void InitializeBackground();

private:
	static constexpr int32 TotalHeight = 40;
	static constexpr int32 VisibleHeight = 20;
	static constexpr int32 TotalWidth = 10;
	static const FString BackgroundMinoMaterialPath;

	UPROPERTY(EditDefaultsOnly)
	ETetriminoType TestType = ETetriminoType::S;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMino> MinoClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<AMino>> Background;
};
