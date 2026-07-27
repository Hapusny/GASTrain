// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "C_AttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) 

/**
 * 
 */
UCLASS()
class GLYPH_API UC_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;

	ATTRIBUTE_ACCESSORS(ThisClass, Health);

	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);
};
