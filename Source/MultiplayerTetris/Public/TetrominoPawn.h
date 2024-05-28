// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/EngineTypes.h"

#include "TetrominoPawn.generated.h"

class ATetromino;
class ATetrisGameModeBase;

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

	void MoveLeft();
	void MoveRight();
	void SoftDrop();
	void HardDrop();

	void UpdateFallSpeed(const float NewFallSpeed);

private:
	void Initialize();
	void SetInitialTimers();
	void OnFallTimer();

private:
	static constexpr bool bIsFallTimerLoop = true;
	static constexpr float FallTimerFirstDelayTime = 0.f;

	// 조작 중인 테트로미노
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetromino> TetrominoInPlay;

	UPROPERTY()
	TObjectPtr<ATetrisGameModeBase> GameMode;

	// 기본 낙하 타이머 핸들러
	FTimerHandle FallTimerHandle;
};
