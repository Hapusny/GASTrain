// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/C_PlayerCharacter.h"
#include "Player/C_PlayerState.h"
#include "UI/C_UIComponent.h"
#include "Glyph/C_GlyphInventoryComponent.h"
#include "AbilitySystemComponent.h"

AC_PlayerCharacter::AC_PlayerCharacter()
{
	UIComponent = CreateDefaultSubobject<UC_UIComponent>("UIComponent");
}

UAbilitySystemComponent* AC_PlayerCharacter::GetAbilitySystemComponent() const
{
	if (!IsValid(GetPlayerState<AC_PlayerState>()))return nullptr;
	return GetPlayerState<AC_PlayerState>()->GetAbilitySystemComponent();
}

void AC_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	GiveStartupAbilities();
}
