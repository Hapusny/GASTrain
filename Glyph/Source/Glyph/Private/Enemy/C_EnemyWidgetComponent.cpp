// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/C_EnemyWidgetComponent.h"
#include "Character/C_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "Enemy/C_EnemyStateWidget.h"

void UC_EnemyWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	BaseCharacter = Cast<AC_BaseCharacter>(GetOwner());
	AttributeSet = Cast<UC_AttributeSet>(BaseCharacter->GetAttributeSet());
	AbilitySystemComponent = Cast<UAbilitySystemComponent>(BaseCharacter->GetAbilitySystemComponent());

	BindToAttributeChanges();
}

void UC_EnemyWidgetComponent::BindToAttributeChanges()
{
	for (const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair : AttributeMap)
	{
		UC_EnemyStateWidget* AttributeWidget = Cast<UC_EnemyStateWidget>(GetUserWidgetObject());
		if (!IsValid(AttributeWidget))return;
		if (!AttributeWidget->MatchesAttributes(Pair))return;

		AttributeWidget->AvatarActor = BaseCharacter;

		AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get(), 0.f);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda([this, AttributeWidget, &Pair](const FOnAttributeChangeData& AttributeChangeData)
		{
			AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get(), AttributeChangeData.OldValue);
		});
	}
}
