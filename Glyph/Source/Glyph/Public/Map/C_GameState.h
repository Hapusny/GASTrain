// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "C_Pheromone.h"
#include "C_GameState.generated.h"

/**
 * 
 */
UCLASS()
class GLYPH_API AC_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void AddPheromone(AC_Pheromone* Pheromone);

	void RemovePheromone(AC_Pheromone* Pheromone);

	TArray<AC_Pheromone*> FindPheromonesNear(FVector Location, float Radius);

private:
	UPROPERTY()
	TArray<TObjectPtr<AC_Pheromone>>Pheromones;

	
};
