// Fill out your copyright notice in the Description page of Project Settings.


#include "Glyph/C_GlyphBase.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UC_GlyphBase::ActivateGlyph(UGameplayAbility* Ability)
{
    if (!IsValid(Ability)) return;
    OwningAbility = MakeWeakObjectPtr<UGameplayAbility>(Ability);

    if (GlyphType == EGlyphType::AttackBase)AttackBase();
    if (GlyphType == EGlyphType::SkillBase)SkillBase();
    if (GlyphType == EGlyphType::MoveBase)MoveBase();
}

void UC_GlyphBase::BaseEvent(EBaseEventType EventType, FBaseEventContext Context)
{
    OnBaseEvent.Broadcast(EventType, Context);
}

void UC_GlyphBase::BaseEventReceived_Implementation(EBaseEventType EventType, FBaseEventContext Context)
{
    
}

UAbilityTask_WaitGameplayEvent* UC_GlyphBase::CreateWaitGameplayEventTask(FGameplayTag EventTag, AActor* OptionalExternalTarget, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
    if (!OwningAbility.IsValid() || !EventTag.IsValid())return nullptr;

    // 直接调用工厂函数生成，激活并返回
    UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(OwningAbility.Get(), EventTag, OptionalExternalTarget, OnlyTriggerOnce, OnlyMatchExact);
    Task->Activate();
    return Task;
}

UAbilityTask_PlayMontageAndWait* UC_GlyphBase::CreatePlayMontageAndWaitTask(FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale, float StartTimeSeconds, bool bAllowInterruptAfterBlendOut)
{
    if (!OwningAbility.IsValid())return nullptr;

    // 直接调用工厂函数生成，激活并返回
    UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(OwningAbility.Get(), TaskInstanceName, MontageToPlay, Rate, StartSection, bStopWhenAbilityEnds, AnimRootMotionTranslationScale, StartTimeSeconds, bAllowInterruptAfterBlendOut);
    Task->Activate();
    return Task;
}

AActor* UC_GlyphBase::SpawnActor(TSubclassOf<AActor> ActorClass, FVector Location, FRotator Rotator)
{
    UWorld* World = GetWorld();
    if (!IsValid(World))return nullptr;

    FActorSpawnParameters SpawnParams;
    if (!OwningAbility.IsValid())return nullptr;
    SpawnParams.Owner = OwningAbility.Get()->GetAvatarActorFromActorInfo();

    //在指定位置和旋转生成
    FVector SpawnLocation = Location;
    FRotator SpawnRotation = Rotator;
    AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClass, SpawnLocation, SpawnRotation, SpawnParams);
    return SpawnedActor;
}
