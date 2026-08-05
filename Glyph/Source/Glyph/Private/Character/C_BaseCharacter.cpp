// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/C_BaseCharacter.h"
#include "Glyph/C_GlyphInventoryComponent.h"
#include "Glyph/C_TravelEventComponent.h"
#include "AbilitySystemComponent.h"

// Sets default values
AC_BaseCharacter::AC_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	GlyphInventoryComponent = CreateDefaultSubobject<UC_GlyphInventoryComponent>("GlyphInventoryComponent");
	TravelEventComponent = CreateDefaultSubobject<UC_TravelEventComponent>("TravelEventComponent");
}

UAbilitySystemComponent* AC_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

UAttributeSet* AC_BaseCharacter::GetAttributeSet() const
{
	return nullptr;
}

void AC_BaseCharacter::GiveStartupAbilities()
{
	if (!IsValid(GetAbilitySystemComponent()))return;
	for (const auto& Ability : StartupAbilities) {
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}
