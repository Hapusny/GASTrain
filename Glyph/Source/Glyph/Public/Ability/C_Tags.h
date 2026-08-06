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

	namespace Datas
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Duration);
	}
}