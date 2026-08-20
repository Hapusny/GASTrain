// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "C_BaseCharacter.generated.h"


class UC_GlyphInventoryComponent;
class UC_TravelEventComponent;
class UGameplayAbility;
class UAttributeSet;

UCLASS()
class GLYPH_API AC_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AC_BaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;

	virtual UAttributeSet* GetAttributeSet()const;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UC_GlyphInventoryComponent>GlyphInventoryComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UC_TravelEventComponent>TravelEventComponent;

	UPROPERTY(EditDefaultsOnly, Category = "C|Ability")
	TArray<TSubclassOf<UGameplayAbility>>StartupAbilities;

	virtual void HandleDeath();

protected:
	void GiveStartupAbilities();

};
