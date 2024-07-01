// Fill out your copyright notice in the Description page of Project Settings.


#include "Tetrimino.h"

ATetrimino::ATetrimino()
	: bIsGhostPieceOn(false)
	, GhostPiece(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}
