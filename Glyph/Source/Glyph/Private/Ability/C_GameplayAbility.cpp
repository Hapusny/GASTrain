// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/C_GameplayAbility.h"
#include "Character/C_BaseCharacter.h"
#include "Glyph/C_GlyphInventoryComponent.h"
#include "Glyph/C_GlyphBase.h"

void UC_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AC_BaseCharacter* BaseCharacter = Cast<AC_BaseCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(BaseCharacter))return;
	UC_GlyphInventoryComponent* GIC = BaseCharacter->GlyphInventoryComponent;
	if (!IsValid(GIC))return;
	if(!GIC->ActivateSlotGlyph(SlotType,this))EndAbility(Handle, ActorInfo, ActivationInfo,false,true);
}
