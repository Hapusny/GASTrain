// Fill out your copyright notice in the Description page of Project Settings.


#include "Glyph/C_GlyphBase.h"
#include "Ability/C_GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Glyph/C_GlyphSpawnActor.h"
#include "Character/C_BaseCharacter.h"
#include "AbilitySystemComponent.h"

void UC_GlyphBase::ActivateGlyph(const FGlyphConfigurationContext& Context)
{
    if (GlyphType == EGlyphType::AttackBase)AttackBase(Context);
    if (GlyphType == EGlyphType::SkillBase)SkillBase(Context);
    if (GlyphType == EGlyphType::MoveBase)MoveBase(Context);
}

void UC_GlyphBase::BaseEvent(EBaseEventType EventType, const FGlyphEventContext& Context)
{
    OnBaseEvent.Broadcast(EventType, Context);
}

void UC_GlyphBase::TryEndAbility()
{
    if (!OwningAbility.IsValid())return;
    UC_GameplayAbility* Ability = Cast<UC_GameplayAbility>(OwningAbility.Get());
    if (!IsValid(Ability))return;
    if (Ability->IsActive())Ability->EndAbilityFormRef();
}

void UC_GlyphBase::InitializeActorByContext(AC_GlyphSpawnActor* Actor, const FGlyphSpawnActorContext& Context)
{
    if (!IsValid(Actor))return;
    Actor->SpawnActorType = Context.SpawnType;
    Actor->Damage = Context.Damage;
    Actor->DamageAttribute = Context.DamageAttribute;
    Actor->FireSpeed = Context.FireSpeed;
    Actor->bCanPenetrate = Context.bCanPenetrate;
}

UAbilityTask_WaitGameplayEvent* UC_GlyphBase::CreateWaitGameplayEventTask(FGameplayTag EventTag, AActor* OptionalExternalTarget, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
    if (!OwningAbility.IsValid() || !EventTag.IsValid())return nullptr;

    // 直接调用工厂函数生成，激活并返回
    UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(OwningAbility.Get(), EventTag, OptionalExternalTarget, OnlyTriggerOnce, OnlyMatchExact);
    if(IsValid(Task))Task->ReadyForActivation();
    return Task;
}

UAbilityTask_PlayMontageAndWait* UC_GlyphBase::CreatePlayMontageAndWaitTask(FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale, float StartTimeSeconds, bool bAllowInterruptAfterBlendOut)
{
    if (!OwningAbility.IsValid())return nullptr;

    // 直接调用工厂函数生成，激活,绑定默认结束ability并返回
    UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(OwningAbility.Get(), TaskInstanceName, MontageToPlay, Rate, StartSection, bStopWhenAbilityEnds, AnimRootMotionTranslationScale, StartTimeSeconds, bAllowInterruptAfterBlendOut);
    if (IsValid(Task)) {
        Task->ReadyForActivation();
        Task->OnBlendOut.AddUniqueDynamic(this, &ThisClass::TryEndAbility);
        Task->OnCancelled.AddUniqueDynamic(this, &ThisClass::TryEndAbility);
        Task->OnCompleted.AddUniqueDynamic(this, &ThisClass::TryEndAbility);
        Task->OnInterrupted.AddUniqueDynamic(this, &ThisClass::TryEndAbility);
    }
    return Task;
}

TArray<AActor*> UC_GlyphBase::SpawnActor(TSubclassOf<AC_GlyphSpawnActor> ActorClass, const FGlyphSpawnActorContext& GlyphSpawnActorContext)
{
    UWorld* World = GetWorld();
    TArray<AActor*> Actors;
    if (!IsValid(World))return Actors;

    if (!IsValid(ActorClass))return Actors;

    if (!OwningAbility.IsValid()) return Actors;
    AC_BaseCharacter* Owner = Cast<AC_BaseCharacter>(OwningAbility.Get()->GetAvatarActorFromActorInfo());
    if (!IsValid(Owner))return Actors;

    //生成
    AC_GlyphSpawnActor* SpawnedActor;

    FTransform Transform;
    if (!GlyphSpawnActorContext.bOverrideOrigin)Transform.SetLocation(Owner->GetActorLocation());
    else Transform.SetLocation(GlyphSpawnActorContext.Origin);
    Transform.SetLocation(Transform.GetLocation() + Owner->GetActorForwardVector() * GlyphSpawnActorContext.ForwardOffset + FVector(0.f, 0.f, GlyphSpawnActorContext.ElevationOffset));

    if (!GlyphSpawnActorContext.bOverrideRotation)Transform.SetRotation(Owner->GetActorRotation().Quaternion());
    else Transform.SetRotation(GlyphSpawnActorContext.Rotation);

    Transform.SetScale3D(GlyphSpawnActorContext.Size);

    switch (GlyphSpawnActorContext.SpawnType)
    {
    case EGlyphSpawnActorType::None:
        SpawnedActor = World->SpawnActorDeferred<AC_GlyphSpawnActor>(ActorClass, Transform, Owner, Owner, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
        if (SpawnedActor) {
            SpawnedActor->Glyph = TWeakObjectPtr<UC_GlyphBase>(this);
            InitializeActorByContext(SpawnedActor, GlyphSpawnActorContext);

            SpawnedActor->FinishSpawning(Transform);
        }
        Actors.Add(SpawnedActor);
        break;

    case EGlyphSpawnActorType::Orbit:
        for (int i = 0; i < GlyphSpawnActorContext.Number; i++) {
            SpawnedActor = World->SpawnActorDeferred<AC_GlyphSpawnActor>(ActorClass, Transform, Owner, Owner, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
            if (SpawnedActor) {
                SpawnedActor->Glyph = TWeakObjectPtr<UC_GlyphBase>(this);
                InitializeActorByContext(SpawnedActor, GlyphSpawnActorContext);
                SpawnedActor->SetOrbit(GlyphSpawnActorContext.OrbitDistance, GlyphSpawnActorContext.OrbitAngleSpeed, 360.f / GlyphSpawnActorContext.Number * i);

                SpawnedActor->FinishSpawning(Transform);
            }
            if (SpawnedActor) Actors.Add(SpawnedActor);
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

TArray<AActor*> UC_GlyphBase::BoxCollisionOverlapCheck(AActor* AvatarActor, FVector BoxSize, float HitBoxForwardOffset, float HitBoxElevationOffset, bool bDrawDebugs)
{
    if (!IsValid(AvatarActor))return TArray<AActor*>();

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(AvatarActor);

    FCollisionResponseParams ResponseParams;
    ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
    ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

    TArray<FOverlapResult> OverlapResults;
    FCollisionShape Box = FCollisionShape::MakeBox(BoxSize);

    const FVector Foward = AvatarActor->GetActorForwardVector() * HitBoxForwardOffset;
    const FVector HitBoxLocation = AvatarActor->GetActorLocation() + Foward + FVector(0.f, 0.f, HitBoxElevationOffset);

    UWorld* World = AvatarActor->GetWorld();
    if (!IsValid(World))return TArray<AActor*>();
    World->OverlapMultiByChannel(OverlapResults, HitBoxLocation, FQuat::Identity, ECC_Pawn, Box, QueryParams, ResponseParams);

    TArray<AActor*> ActorsHit;

    for (const FOverlapResult& Result : OverlapResults) {
        AC_BaseCharacter* BaseCharacter = Cast<AC_BaseCharacter>(Result.GetActor());
        if (!IsValid(Result.GetActor()))continue;
        ActorsHit.AddUnique(BaseCharacter);
    }

    if (bDrawDebugs)
    {
        DrawDebugBox(World, HitBoxLocation, BoxSize,FColor::Red, false, 3.f);

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

TArray<AActor*> UC_GlyphBase::LineCollisionHitCheck(AActor* AvatarActor, float Length, float HitBoxForwardOffset, float HitBoxElevationOffset, bool bDrawDebugs)
{
    if (!IsValid(AvatarActor))return TArray<AActor*>();

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(AvatarActor);

    FCollisionResponseParams ResponseParams;
    ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
    ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Overlap);

    TArray<FHitResult> HitResults;

    const FVector Foward = AvatarActor->GetActorForwardVector() * HitBoxForwardOffset;
    const FVector Start = AvatarActor->GetActorLocation() + Foward + FVector(0.f, 0.f, HitBoxElevationOffset);
    const FVector End = Start + AvatarActor->GetActorForwardVector() * Length;

    UWorld* World = AvatarActor->GetWorld();
    if (!IsValid(World))return TArray<AActor*>();
    World->LineTraceMultiByChannel(HitResults, Start,End, ECC_Pawn, QueryParams, ResponseParams);

    TArray<AActor*> ActorsHit;

    for (const FHitResult& Result : HitResults) {
        AC_BaseCharacter* BaseCharacter = Cast<AC_BaseCharacter>(Result.GetActor());
        if (!IsValid(Result.GetActor()))continue;
        ActorsHit.AddUnique(BaseCharacter);
    }

    if (bDrawDebugs)
    {
        DrawDebugLine(World, Start, End, FColor::Blue, false, 2.0f, 0, 2.0f);

        for (const FHitResult& Result : HitResults) {
            if (IsValid(Result.GetActor())) {
                FVector DebugLocation = Result.GetActor()->GetActorLocation();
                DebugLocation.Z += 100.f;
                DrawDebugSphere(World, DebugLocation, 30.f, 10, FColor::Green, false, 3.f);
            }
        }
    }
    return ActorsHit;
}

FGlyphConfigurationContext UC_GlyphBase::PreProcessContext_Implementation(EGlyphType BaseGlyphType, FGlyphConfigurationContext Context)
{
    return Context;
}




