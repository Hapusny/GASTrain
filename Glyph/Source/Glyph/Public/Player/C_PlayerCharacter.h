// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/C_BaseCharacter.h"
#include "C_PlayerCharacter.generated.h"

class UC_UIComponent;
/**
 * 
 */
UCLASS()
class GLYPH_API AC_PlayerCharacter : public AC_BaseCharacter
{
	GENERATED_BODY()
	
public:
	AC_PlayerCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;

	virtual void BeginPlay()override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UC_UIComponent>UIComponent;
};
