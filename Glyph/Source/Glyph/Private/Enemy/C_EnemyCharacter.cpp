// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/C_EnemyCharacter.h"
#include "Ability/C_AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Glyph/C_GlyphInventoryComponent.h"

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

void AC_EnemyCharacter::GiveGlyph(int32 Level)
{
    TArray<TSubclassOf<UC_GlyphBase>> SelectedGlyphs = GetMultipleRandomWeightedGlyphs(GlyphPool, Level);
    EGlyphType SlotTypes[] = {EGlyphType::AttackBase, EGlyphType::SkillBase,EGlyphType::MoveBase, EGlyphType::AttackVariant,EGlyphType::SkillVariant,EGlyphType::MoveVariant};

    for (int32 i = 0; i < SelectedGlyphs.Num() && i < 6; i++)
    {
        UC_GlyphBase* Glyph = GlyphInventoryComponent->AddGlyph(SelectedGlyphs[i]);
        if (Glyph)GlyphInventoryComponent->SetGlyphType(Glyph, SlotTypes[i]);
    }
}

void AC_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(GetAbilitySystemComponent()))return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	GiveStartupAbilities();
}

TArray<TSubclassOf<UC_GlyphBase>> AC_EnemyCharacter::GetMultipleRandomWeightedGlyphs(TMap<TSubclassOf<UC_GlyphBase>, float> Pool, int32 Count, bool bAllowFallback)
{
    TArray<TSubclassOf<UC_GlyphBase>> Result;
    if (Count <= 0 || Pool.Num() == 0)return Result;
    if (!bAllowFallback && Pool.Num() < Count)return Result;

    int32 ActualCount = FMath::Min(Count, Pool.Num());

    for (int32 i = 0; i < ActualCount; i++)
    {
        // 计算当前池子的总权重
        float TotalWeight = 0.0f;
        for (const auto& Pair : Pool)TotalWeight += Pair.Value;

        if (TotalWeight <= 0.0f)break;

        // 加权随机选择
        float RandomValue = FMath::FRandRange(0.0f, TotalWeight);
        float AccumulatedWeight = 0.0f;
        TSubclassOf<UC_GlyphBase> SelectedClass = nullptr;

        for (const auto& Pair : Pool)
        {
            AccumulatedWeight += Pair.Value;
            if (RandomValue <= AccumulatedWeight)
            {
                SelectedClass = Pair.Key;
                break;
            }
        }
        if (SelectedClass != nullptr)
        {
            Result.Add(SelectedClass);
            Pool.Remove(SelectedClass);
        }
    }
    return Result;
}
