// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/C_Tags.h"

namespace CTags {
	namespace Abilities
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attack, "CTags.Abilities.Attack", "Tag for the Attack Ability");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill, "CTags.Abilities.Skill", "Tag for the Skill Ability");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Move, "CTags.Abilities.Move", "Tag for the Move Ability");
	}

	namespace Events
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Hit, "CTags.Events.Hit", "Tag for Event Hit");
	}

	namespace States
	{
		namespace Buffs
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageReduction, "CTags.States.Buffs.DamageReduction", "Tag for Buff DamageReduction");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Block, "CTags.States.Buffs.Block", "Tag for Buff Block");
		}

		namespace Debuffs
		{

		}
	}

	namespace Datas 
	{
		namespace Elements
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fire, "CTags.Datas.Elements.Fire", "Tag for Elements Data Fire");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Water, "CTags.Datas.Elements.Water", "Tag for Elements Data Water");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Wind, "CTags.Datas.Elements.Wind", "Tag for Elements Data Wind");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Soil, "CTags.Datas.Elements.Soil", "Tag for Elements Data Soil");
		}

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage, "CTags.Datas.Damage", "Tag for Data Damage");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Duration, "CTags.Datas.Duration", "Tag for Data Duration");
	}
}
