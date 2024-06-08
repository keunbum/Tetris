// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Tetrimino.h"

#include "TetrisPlayManager.generated.h"

class ATetrimino;
class ATetrisGameModeBase;
class ABoard;
class UInputComponent;

UCLASS()
class MULTIPLAYERTETRIS_API ATetrisPlayManager : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATetrisPlayManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

public:
	void StartGenerationPhase();
	void SetTetriminoInPlay(ATetrimino* const NewTetriminoInPlay) { TetriminoInPlay = NewTetriminoInPlay; }
	void SetNormalFallSpeed(const float NewNormalFallSpeed) { NormalFallSpeed = NewNormalFallSpeed; }

	FVector2D GetMovementDirection() const { return CurrentMovementDirection; }

	// Event Handlers
	void StartMovement(const FVector2D& InMovementDirection);
	void EndMovement();
	void StartSoftDrop();
	void EndSoftDrop();
	void StartHardDrop();
	/**
	* @param RotationDirection - +1: 시계 방향, -1: 반시계 방향.
	*/
	void StartRotate(const int32 RotationDirection);

private:
	void Initialize();
	void ClearTimer(FTimerHandle& InOutTimerHandle);

	void MoveTetriminoTo(const FVector2D& Direction);
	void MoveTetriminoToCurrentDirection();
	void MoveDown();

	void SetAutoRepeatMovement();
	void SetNormalFallTimer();

	void SetMovementDirection(const FVector2D& NewMovementDirection) { CurrentMovementDirection = NewMovementDirection; }

	ATetrimino* SpawnNextTetrimino() const;
	void AttachTetrimino(ATetrimino* const NewTetrimino);

	static bool IsNextPositionPossible(const ATetrimino* Tetrimino, const FVector2D& MovementVector2D);
	static bool IsWithinRange(const ATetrimino* Tetrimino);

private:
	static constexpr bool bIsNormalFallTimerLoop = true;
	static constexpr float NormalFallTimerInitialDelay = 0.0f;

	static constexpr bool bSoftDropTimerLoop = true;
	static constexpr float SoftDropTimerInitialDelay = 0.0f;

	static constexpr bool bIsAutoRepeatMovementLoop = true;
	static constexpr float AutoRepeatMovementInitialDelay = 0.3f;
	static constexpr float AutoRepeatMovementInterval = 0.05f; // Adjust this value as needed

private:
	float NormalFallSpeed;

	UPROPERTY()
	TObjectPtr<ATetrisGameModeBase> GameMode;

	UPROPERTY()
	TObjectPtr<ABoard> Board;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATetrimino> TetriminoClass;

	// 조작 중인 테트로미노
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetrimino> TetriminoInPlay;

	FTimerHandle NormalFallTimerHandle;
	FTimerHandle SoftDropTimerHandle;
	FTimerHandle AutoRepeatMovementTimerHandle;

	UPROPERTY(VisibleAnywhere)
	FVector2D CurrentMovementDirection;
};
