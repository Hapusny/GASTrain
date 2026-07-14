// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/C_PlayerCharacter.h"
#include "Player/C_PlayerState.h"

UAbilitySystemComponent* AC_PlayerCharacter::GetAbilitySystemComponent() const
{
	if (!IsValid(GetPlayerState<AC_PlayerState>()))return nullptr;
	return GetPlayerState<AC_PlayerState>()->GetAbilitySystemComponent();
}
