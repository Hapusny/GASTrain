// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/C_EnemyCharacter.h"
#include "Ability/C_AttributeSet.h"
#include "AbilitySystemComponent.h"

AC_EnemyCharacter::AC_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UC_AttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AC_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AC_EnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void AC_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(GetAbilitySystemComponent()))return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	GiveStartupAbilities();
}