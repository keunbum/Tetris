// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/EngineTypes.h"

#include "TetrominoPawn.generated.h"

class ATetromino;
class ATetrisGameModeBase;
class UWorld;

UCLASS()
class MULTIPLAYERTETRIS_API ATetrominoPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATetrominoPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void SetTetrominoInPlay(ATetromino* const NewTetrominoInPlay);

	void OnMoveLeft();
	void OnMoveRight();
	void OnSoftDrop();
	void OnStopSoftDrop();
	void OnHardDrop();

	void UpdateNormalFallSpeed(const float NewFallSpeed);

private:
	void Initialize();
	void SetInitialTimers();
	void ClearTimer(FTimerHandle& InOutTimerHandle);
	void SetFallTimer(FTimerHandle& InOutFallTimerHandle, const float NewFallSpeed, const bool bIsTimerLoop, const float FirstDelayTime);
	void OnFallTimer();

private:
	static constexpr bool bIsNormalFallTimerLoop = true;
	static constexpr float NormalFallTimerFirstDelayTime = 0.f;
	static const bool bSoftDropTimerLoop = true;
	static constexpr float SoftDropTimerFirstDelayTime = 0.f;

	UPROPERTY()
	TObjectPtr<ATetrisGameModeBase> GameMode;

	// 조작 중인 테트로미노
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetromino> TetrominoInPlay;

	// 타이머 핸들러
	FTimerHandle NormalFallTimerHandle;
	FTimerHandle SoftDropTimerHandle;
};
