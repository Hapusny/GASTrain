// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/C_AttributeSet.h"
#include "Character/C_BaseCharacter.h"
#include "Glyph/C_GlyphBase.h"
#include "GameplayEffectExtension.h"
#include "Ability/C_Tags.h"

void UC_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		// 1. 读取原始伤害值
		float RawDamage = GetIncomingDamage();

		SetIncomingDamage(0.f);

		if (RawDamage <= 0.f) return;

		float FinalDamage = CalculateFinalDamage(Data, RawDamage);
		if (FinalDamage <= 0.f) return;

		// 应用最终伤害到 Health
		float OldHealth = GetHealth();
		float NewHealth = OldHealth - FinalDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		// 死亡判定
		if (GetHealth() <= 0.f)
		{
			// 获取伤害来源
			AActor* Instigator = Data.EffectSpec.GetContext().GetInstigator();

			// 提取伤害来源Glyph
			UC_GlyphBase* SourceGlyph = Cast<UC_GlyphBase>(Data.EffectSpec.GetContext().GetSourceObject());

			// 调用死亡处理函数
		}
	}
}

void UC_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		// 血量限制在 [0, MaxHealth] 区间
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// 最大血量不能低于1
		NewValue = FMath::Max(NewValue, 1.f);
	}
	else if (Attribute == GetFireResistanceAttribute()
		|| Attribute == GetWaterResistanceAttribute()
		|| Attribute == GetWindResistanceAttribute()
		|| Attribute == GetSoilResistanceAttribute())
	{
		// 抗性限制在 [0, 1] 区间
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
}

float UC_AttributeSet::CalculateFinalDamage(const FGameplayEffectModCallbackData& Data, float RawDamage) const
{
	if (RawDamage <= 0.f) return 0.f;

	//获取元素
	FGameplayTagContainer AllAssetTags;
	Data.EffectSpec.GetAllAssetTags(AllAssetTags);

	FGameplayTag ElementType;
	for (const FGameplayTag& Tag : AllAssetTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("CTags.Datas.Elements")))
		{
			ElementType = Tag;
			break;
		}
	}

	//根据元素类型应用抗性
	float Resistance = 0.f;
	if (ElementType == CTags::Datas::Elements::Fire)
	{
		Resistance = GetFireResistance();
	}
	else if (ElementType == CTags::Datas::Elements::Water)
	{
		Resistance = GetWaterResistance();
	}
	else if (ElementType == CTags::Datas::Elements::Wind)
	{
		Resistance = GetWindResistance();
	}
	else if (ElementType == CTags::Datas::Elements::Soil)
	{
		Resistance = GetSoilResistance();
	}

	float FinalDamage = RawDamage * (1.f - Resistance);

	// 检测Tag
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (ASC && ASC->HasMatchingGameplayTag(CTags::States::Buffs::Block))
	{
		FinalDamage = 0.f;
	}
	if (ASC && ASC->HasMatchingGameplayTag(CTags::States::Buffs::DamageReduction))
	{
		FinalDamage *= 0.6f;
	}

	return FMath::Max(0.f, FinalDamage);
}
