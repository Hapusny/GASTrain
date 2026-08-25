// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "C_Pheromone.h"
#include "C_GameMode.generated.h"

class AC_EnemyCharacter;

/**
 * 
 */
UCLASS()
class GLYPH_API AC_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "C|GameMode")
	AC_EnemyCharacter* SpawnEnemy(TSubclassOf<AC_EnemyCharacter> ActorClass,FVector Location, TArray<float>Values);

	UPROPERTY(EditDefaultsOnly, Category = "C|GameMode")
	TSubclassOf<AC_Pheromone> PheromoneClass;

	UFUNCTION(BlueprintCallable, Category = "C|GameMode")
	void SpawnPheromone(FVector Location);
};
