// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "C_PlayerState.generated.h"

class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class GLYPH_API AC_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AC_PlayerState();

	UAbilitySystemComponent* GetAbilitySystemComponent();

private:
	UPROPERTY(VisibleAnywhere, Category = "C|Abilities")
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;
	
};
