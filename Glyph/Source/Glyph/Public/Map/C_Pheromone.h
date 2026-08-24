// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Pheromone.generated.h"

UCLASS()
class GLYPH_API AC_Pheromone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_Pheromone();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C|Actor")
	float Radius = 200.f;
};
