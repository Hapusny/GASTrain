// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/C_PlayerState.h"
#include "AbilitySystemComponent.h"
#include "Ability/C_AttributeSet.h"

AC_PlayerState::AC_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");

	AttributeSet = CreateDefaultSubobject<UC_AttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AC_PlayerState::GetAbilitySystemComponent()const
{
	return AbilitySystemComponent;
}

UAttributeSet* AC_PlayerState::GetAttributeSet()const
{
	return AttributeSet;
}
