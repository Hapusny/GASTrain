// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/C_BaseCharacter.h"

// Sets default values
AC_BaseCharacter::AC_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

}

UAbilitySystemComponent* AC_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}
