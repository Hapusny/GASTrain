// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/C_PlayerState.h"
#include "AbilitySystemComponent.h"

AC_PlayerState::AC_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
}

UAbilitySystemComponent* AC_PlayerState::GetAbilitySystemComponent() 
{
	return AbilitySystemComponent;
}