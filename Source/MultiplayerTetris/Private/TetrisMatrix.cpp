// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisMatrix.h"

// Sets default values
ATetrisMatrix::ATetrisMatrix()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATetrisMatrix::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATetrisMatrix::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

