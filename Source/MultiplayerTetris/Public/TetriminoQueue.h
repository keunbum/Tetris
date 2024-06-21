// Copyright KeunBeom Ryu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TetriminoQueue.generated.h"

UCLASS()
class MULTIPLAYERTETRIS_API ATetriminoQueue : public AActor
{
	GENERATED_BODY()
	
public:	
	ATetriminoQueue();

protected:
	virtual void BeginPlay() override;

private:
};
