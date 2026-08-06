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

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData FireResistance;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData WaterResistance;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData WindResistance;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData SoilResistance;

	ATTRIBUTE_ACCESSORS(ThisClass, Health);

	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);

	ATTRIBUTE_ACCESSORS(ThisClass, FireResistance);

	ATTRIBUTE_ACCESSORS(ThisClass, WaterResistance);

	ATTRIBUTE_ACCESSORS(ThisClass, WindResistance);

	ATTRIBUTE_ACCESSORS(ThisClass, SoilResistance);
};
