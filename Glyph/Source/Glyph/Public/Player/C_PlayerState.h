// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "C_PlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class GLYPH_API AC_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AC_PlayerState();

	UAbilitySystemComponent* GetAbilitySystemComponent()const;

	UAttributeSet* GetAttributeSet()const;

private:
	UPROPERTY(VisibleAnywhere, Category = "C|Ability")
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;
	
};
