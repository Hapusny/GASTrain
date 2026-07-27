// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ability/C_AttributeSet.h"
#include "C_EnemyStateWidget.generated.h"

/**
 * 
 */
UCLASS()
class GLYPH_API UC_EnemyStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C|Attributes")
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C|Attributes")
	FGameplayAttribute MaxAttribute;

	bool MatchesAttributes(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair)const;

	void OnAttributeChange(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair, UC_AttributeSet* AttributeSet, float OldValue);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Attribute Change"),Category = "C|Attributes")
	void BP_OnAttributeChange(float NewValue, float NewMaxValue, float OldValue);

	UPROPERTY(BlueprintReadOnly, Category = "C|Attributes")
	TWeakObjectPtr<AActor>AvatarActor;
};
