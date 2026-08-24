// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/C_GameState.h"

void AC_GameState::AddPheromone(AC_Pheromone* Pheromone)
{
    if (!IsValid(Pheromone))return;
    if (!Pheromones.Contains(Pheromone))
    {
        Pheromones.Add(Pheromone);
    }
}

void AC_GameState::RemovePheromone(AC_Pheromone* Pheromone)
{
    if (!IsValid(Pheromone))return;
    Pheromones.Remove(Pheromone);
}

TArray<AC_Pheromone*> AC_GameState::FindPheromonesNear(FVector Location, float Radius)
{
    TArray<AC_Pheromone*> Result;

    for (const TObjectPtr<AC_Pheromone>& Pheromone : Pheromones)
    {
        if (!IsValid(Pheromone))continue;

        float DistSq = FVector::DistSquared(Pheromone->GetActorLocation(), Location);
        if (DistSq <= Radius * Radius)
        {
            Result.Add(Pheromone);
        }
    }

    return Result;
}
