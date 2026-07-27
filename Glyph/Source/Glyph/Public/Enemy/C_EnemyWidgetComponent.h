// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "Ability/C_AttributeSet.h"
#include "C_EnemyWidgetComponent.generated.h"


class AC_BaseCharacter;
class UC_AttributeSet;
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class GLYPH_API UC_EnemyWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay()override;

	UPROPERTY(EditDefaultsOnly, Category = "C|Attributes")
	TMap<FGameplayAttribute, FGameplayAttribute>AttributeMap;

private:
	UPROPERTY()
	TWeakObjectPtr<AC_BaseCharacter> BaseCharacter;

	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TWeakObjectPtr<UC_AttributeSet>AttributeSet;

	UFUNCTION()
	void BindToAttributeChanges();
	
};
