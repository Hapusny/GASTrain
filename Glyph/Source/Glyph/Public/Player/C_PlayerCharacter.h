// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/C_BaseCharacter.h"
#include "C_PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GLYPH_API AC_PlayerCharacter : public AC_BaseCharacter
{
	GENERATED_BODY()
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;
};
