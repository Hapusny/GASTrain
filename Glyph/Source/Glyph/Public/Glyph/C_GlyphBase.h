// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "C_GlyphBase.generated.h"

UENUM(BlueprintType)
enum class EGlyphType : uint8
{
	None,
	AttackBase,
	AttackVariant,
	SkillBase,
	SkillVariant,
	MoveBase,
	MoveVariant,
};

/**
 * 
 */
UCLASS()
class GLYPH_API UC_GlyphBase : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "C|Glyph")
	FName GlyphName = FName("Base");

	EGlyphType GlyphType = EGlyphType::None;
};
