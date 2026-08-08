// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CTags 
{
	namespace Abilities
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Move);
	}

	namespace Events
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Hit);
	}

	namespace States
	{
		namespace Buffs
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageReduction);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Block);
		}

		namespace Debuffs
		{

		}
	}

	namespace Datas
	{
		namespace Elements
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fire);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Water);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Wind);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Soil);
		}

		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Duration);
	}
}