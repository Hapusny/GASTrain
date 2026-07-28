// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_GlyphSpawnActor.generated.h"

class UC_GlyphBase;

UCLASS()
class GLYPH_API AC_GlyphSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AC_GlyphSpawnActor();

	UPROPERTY(BlueprintReadWrite, Category = "C|Glyph")
	TWeakObjectPtr<UC_GlyphBase>Glyph;
};
