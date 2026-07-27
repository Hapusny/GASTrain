// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/C_EnemyStateWidget.h"

bool UC_EnemyStateWidget::MatchesAttributes(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
	return Attribute == Pair.Key && MaxAttribute == Pair.Value;
}

void UC_EnemyStateWidget::OnAttributeChange(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair, UC_AttributeSet* AttributeSet, float OldValue)
{
	const float AttributeValue = Pair.Key.GetNumericValue(AttributeSet);
	const float MaxAttributeValue = Pair.Value.GetNumericValue(AttributeSet);

	BP_OnAttributeChange(AttributeValue, MaxAttributeValue, OldValue);
}
