// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/C_BaseCharacter.h"
#include "C_EnemyCharacter.generated.h"

class UC_GlyphBase;

/**
 * 
 */
UCLASS()
class GLYPH_API AC_EnemyCharacter : public AC_BaseCharacter
{
	GENERATED_BODY()

public:
	AC_EnemyCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;
	virtual UAttributeSet* GetAttributeSet()const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C|Enemy|Glyph")
	TMap<TSubclassOf<UC_GlyphBase>,float>GlyphPool;

	//普攻本位、技能本位、位移本位、普攻变位、技能变位、位移变位分别对应6个等级解锁的槽位
	UFUNCTION(BlueprintCallable, Category = "C|Enemy|Glyph")
	void GiveGlyph(int32 Level);

	virtual void HandleDeath()override;

	UFUNCTION(BlueprintImplementableEvent, Category = "C|Enemy|Death")
	void BP_HandleDeath();

protected:
	virtual void BeginPlay()override;

private:
	UPROPERTY(VisibleAnywhere, Category = "C|Ability")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/*
	 从权重池中一次性抽取 N 个不同的刻印（无放回）
	 GlyphPool        权重池
	 Count            需要抽取的数量
	 bAllowFallback   如果池子不够大，是否允许返回部分结果（false则返回空数组）
	 return           抽取到的刻印类数组
	 */
	static TArray<TSubclassOf<UC_GlyphBase>> GetMultipleRandomWeightedGlyphs(TMap<TSubclassOf<UC_GlyphBase>, float> Pool,int32 Count,bool bAllowFallback = true);
};
