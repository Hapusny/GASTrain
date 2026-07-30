// Fill out your copyright notice in the Description page of Project Settings.


#include "Glyph/C_GlyphBase.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Glyph/C_GlyphSpawnActor.h"
#include "Character/C_BaseCharacter.h"

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

TArray<AActor*> UC_GlyphBase::SpawnActor(UC_GlyphBase* Glyph,TSubclassOf<AC_GlyphSpawnActor> ActorClass, FTransform Transform, int Number, ESpawnActorType SpawnActorType, float OrbitDistance, float OrbitAngleSpeed)
{
    UWorld* World = GetWorld();
    TArray<AActor*> Actors;
    if (!IsValid(World))return Actors;

    if (!IsValid(Glyph))return Actors;
    if (!IsValid(ActorClass))return Actors;

    if (!OwningAbility.IsValid())return Actors;
    AC_BaseCharacter* Owner = Cast<AC_BaseCharacter>(OwningAbility.Get()->GetAvatarActorFromActorInfo());
    if (!IsValid(Owner))return Actors;

    //生成
    AC_GlyphSpawnActor* SpawnedActor;

    switch (SpawnActorType)
    {
    case ESpawnActorType::None:
        SpawnedActor = World->SpawnActorDeferred<AC_GlyphSpawnActor>(ActorClass,Transform,Owner,Owner,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
        if (SpawnedActor) {
            SpawnedActor->Glyph = TWeakObjectPtr<UC_GlyphBase>(Glyph);
            SpawnedActor->SpawnActorType = SpawnActorType;

            SpawnedActor->FinishSpawning(Transform);
        }
        Actors.Add(SpawnedActor);
        break;

    case ESpawnActorType::Orbit:
        for (int i = 0; i < Number; i++) {
            SpawnedActor = World->SpawnActorDeferred<AC_GlyphSpawnActor>(ActorClass, Transform, Owner, Owner, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
            if (SpawnedActor) {
                SpawnedActor->Glyph = TWeakObjectPtr<UC_GlyphBase>(Glyph);
                SpawnedActor->SpawnActorType = SpawnActorType;
                SpawnedActor->SetOrbit(OrbitDistance, OrbitAngleSpeed, 360.f / Number * i);
                
                SpawnedActor->FinishSpawning(Transform);
            }
            Actors.Add(SpawnedActor);
        }
        break;
    default:
        break;
    }
    return Actors;
}

TArray<AActor*> UC_GlyphBase::SphereCollisionOverlapCheck(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset, float HitBoxElevationOffset, bool bDrawDebugs)
{
    if (!IsValid(AvatarActor))return TArray<AActor*>();

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(AvatarActor);

    FCollisionResponseParams ResponseParams;
    ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
    ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

    TArray<FOverlapResult> OverlapResults;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);

    const FVector Foward = AvatarActor->GetActorForwardVector() * HitBoxForwardOffset;
    const FVector HitBoxLocation = AvatarActor->GetActorLocation() + Foward + FVector(0.f, 0.f, HitBoxElevationOffset);

    UWorld* World = AvatarActor->GetWorld();
    if (!IsValid(World))return TArray<AActor*>();
    World->OverlapMultiByChannel(OverlapResults, HitBoxLocation, FQuat::Identity, ECC_Pawn, Sphere, QueryParams, ResponseParams);

    TArray<AActor*> ActorsHit;

    for (const FOverlapResult& Result : OverlapResults) {
        AC_BaseCharacter* BaseCharacter = Cast<AC_BaseCharacter>(Result.GetActor());
        if (!IsValid(Result.GetActor()))continue;
        ActorsHit.AddUnique(BaseCharacter);
    }

    if (bDrawDebugs)
    {
        DrawDebugSphere(World, HitBoxLocation, HitBoxRadius, 16, FColor::Red, false, 3.f);

        for (const FOverlapResult& Result : OverlapResults) {
            if (IsValid(Result.GetActor())) {
                FVector DebugLocation = Result.GetActor()->GetActorLocation();
                DebugLocation.Z += 100.f;
                DrawDebugSphere(World, DebugLocation, 30.f, 10, FColor::Green, false, 3.f);
            }
        }
    }
    return ActorsHit;
}



