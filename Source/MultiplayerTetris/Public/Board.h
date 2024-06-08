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
	void AttachTetrimino(ATetrimino* const NewTetrimino);

private:
	void Initialize();
	void InitializeBackground();
	void InitializeMinoMatrix();

	static UMaterialInterface* GetMinoMaterialByPath(const FString& Path);

public:
	static constexpr int32 TotalHeight = 40;
	static constexpr int32 TotalWidth = 10;

	static constexpr int32 VisibleHeight = 20;
	static constexpr int32 VisibleStartRow = TotalHeight - VisibleHeight;
	static constexpr int32 VisibleStartCol = 0;

	static constexpr int32 TetriminoDefaultSpawnLocationX = ABoard::VisibleStartRow - 1;
	static constexpr int32 TetriminoDefaultSpawnLocationY = ABoard::VisibleStartCol + 3;

private:
	static const FString BackgroundMinoMaterialPath;
	static const FString SpecialMinoMaterialPath;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMino> MinoClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<AMino>> Background;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AMino>> MinoMatrix;
};
