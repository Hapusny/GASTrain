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
	virtual void BeginPlay()override;

	UFUNCTION(BlueprintCallable, Category = "C|GameMode|Enemy")
	AC_EnemyCharacter* SpawnEnemy(TSubclassOf<AC_EnemyCharacter> ActorClass,FVector Location, TArray<float>Values);

	UPROPERTY(EditDefaultsOnly, Category = "C|GameMode|Pheromone")
	TSubclassOf<AC_Pheromone> PheromoneClass;

	UFUNCTION(BlueprintCallable, Category = "C|GameMode|Pheromone")
	void SpawnPheromone(FVector Location);

	UFUNCTION(BlueprintCallable, Category = "C|GameMode|Enemy")
	int32 GetEnemyCount()const { return Enemys.Num(); }

	UFUNCTION(BlueprintCallable, Category = "C|GameMode|Enemy")
	void RemoveEnemy(AC_EnemyCharacter* Enemy);

	UFUNCTION(BlueprintImplementableEvent, Category = "C|GameMode|Enemy")
	void BP_WaveFinished();

private:
	TArray<TWeakObjectPtr<AC_EnemyCharacter>>Enemys;
};
