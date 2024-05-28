// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tetromino.h"

#include "Board.generated.h"

class AMino;
class Atetromino;

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
	void AttachTetromino(ATetromino* const NewTetrominoInPlay);

private:
	void Initialize();
	void SpawnTetromino(const FVector& SpawnLocation, const FRotator& SpawnRotation, const ETetrominoType TetrominoType);
	void SpawnTetrominos();

private:
	static constexpr int32 TotalHeight = 40;
	static constexpr int32 VisibleHeight = 20;
	static constexpr int32 Width = 10;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMino> MinoClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATetromino> TetrominoClass = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetromino> TetrominoInPlay;
};
