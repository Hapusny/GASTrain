// Fill out your copyright notice in the Description page of Project Settings.


#include "Glyph/C_GlyphBase.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UC_GlyphBase::BaseEvent_Implementation(EBaseEventType EventType, FBaseEventContext Context)
{
    OnBaseEvent.Broadcast(EventType, Context);
}

void UC_GlyphBase::BaseEventReceived_Implementation(EBaseEventType EventType, FBaseEventContext Context)
{
    
}

void UC_GlyphBase::ActivateGlyph_Implementation(UGameplayAbility* Ability)
{

}

UAbilityTask_WaitGameplayEvent* UC_GlyphBase::CreateWaitGameplayEventTask(UGameplayAbility* OwningAbility, FGameplayTag EventTag, AActor* OptionalExternalTarget, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
    if (!OwningAbility || !EventTag.IsValid())
    {
        return nullptr;
    }

    // 直接调用工厂函数并返回
    UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(OwningAbility, EventTag, OptionalExternalTarget, OnlyTriggerOnce, OnlyMatchExact);
    Task->Activate();
    return Task;
}

UAbilityTask_PlayMontageAndWait* UC_GlyphBase::CreatePlayMontageAndWaitTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale, float StartTimeSeconds, bool bAllowInterruptAfterBlendOut)
{
    if (!OwningAbility)
    {
        return nullptr;
    }

    // 直接调用工厂函数激活并返回
    UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(OwningAbility, TaskInstanceName, MontageToPlay, Rate, StartSection, bStopWhenAbilityEnds, AnimRootMotionTranslationScale, StartTimeSeconds, bAllowInterruptAfterBlendOut);
    Task->Activate();
    return Task;
}
