// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "C_GameplayAbility.generated.h"

enum class EGlyphType : uint8;

/**
 * 
 */
UCLASS()
class GLYPH_API UC_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;

	UPROPERTY(EditDefaultsOnly, Category = "C|Ability")
	EGlyphType SlotType;

	void EndAbilityFormRef();
};
