// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "C_GlyphSpawnActor.h"
#include "C_GlyphBase.generated.h"

class UGameplayAbility;
class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;
class AC_GlyphSpawnActor;

UENUM(BlueprintType)
enum class EGlyphAttribute : uint8
{
	None,
	Fire,
	Water,
	Wind,
	Soil
};

UENUM(BlueprintType)
enum class EGlyphType : uint8
{
	None,
	AttackBase,
	AttackVariant,
	SkillBase,
	SkillVariant,
	MoveBase,
	MoveVariant
};

UENUM(BlueprintType)
enum class EBaseEventType : uint8
{
	OnCast,
	OnSpawn,
	OnTravel,
	OnHit,
	OnFinish
};

USTRUCT(BlueprintType)
struct FBaseEventContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "EventContext")
	AActor* Initiator;

	UPROPERTY(BlueprintReadWrite, Category = "EventContext")
	FVector EventLocation;

	UPROPERTY(BlueprintReadWrite, Category = "EventContext")
	FVector EventToward;

	UPROPERTY(BlueprintReadWrite, Category = "EventContext")
	float Damage;

	UPROPERTY(BlueprintReadWrite, Category = "EventContext")
	TArray<FHitResult>HitResults;

	FBaseEventContext()
		: Initiator(nullptr)
		, EventLocation(FVector::ZeroVector)
		, EventToward(FVector::ZeroVector)
		, Damage(0.0f)
	{
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBaseEventReceived, EBaseEventType, EventType, FBaseEventContext, Context);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class GLYPH_API UC_GlyphBase : public UObject
{
	GENERATED_BODY()
	
public:

	//Glyph Information
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "C|Glyph")
	FName GlyphName = FName("Base");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C|Glyph")
	EGlyphAttribute GlyphAttribute = EGlyphAttribute::None;

	EGlyphType GlyphType = EGlyphType::None;

	UPROPERTY(BlueprintReadOnly, Category = "C|Ability")
	TWeakObjectPtr<UGameplayAbility>OwningAbility;

	//Tool Function
	UFUNCTION(BlueprintCallable, Category = "C|Tool|Tasks")
	UAbilityTask_WaitGameplayEvent* CreateWaitGameplayEventTask(FGameplayTag EventTag, AActor* OptionalExternalTarget = nullptr, bool OnlyTriggerOnce = false, bool OnlyMatchExact = true);

	UFUNCTION(BlueprintCallable, Category = "C|Tool|Tasks")
	UAbilityTask_PlayMontageAndWait* CreatePlayMontageAndWaitTask(FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f, bool bAllowInterruptAfterBlendOut = false);

	UFUNCTION(BlueprintCallable, Category = "C|Tool|Spawn")
	TArray<AActor*> SpawnActor(UC_GlyphBase* Glyph, TSubclassOf<AC_GlyphSpawnActor> ActorClass, FTransform Transform,int Number = 1,ESpawnActorType SpawnActorType = ESpawnActorType::None,float OrbitDistance = 100.f,float OrbitAngleSpeed = 1.f);

	UFUNCTION(BlueprintCallable, Category = "C|Tool|Collision")
	TArray<AActor*> SphereCollisionOverlapCheck(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset = 0.f, float HitBoxElevationOffset = 0.f, bool bDrawDebugs = false);

	//Glyph Ability
	void ActivateGlyph(UGameplayAbility* Ability);

	UFUNCTION(BlueprintImplementableEvent, Category = "C|Glyph|Base")
	void AttackBase();

	UFUNCTION(BlueprintImplementableEvent, Category = "C|Glyph|Base")
	void SkillBase();

	UFUNCTION(BlueprintImplementableEvent, Category = "C|Glyph|Base")
	void MoveBase();

	FBaseEventReceived OnBaseEvent;

	UFUNCTION(BlueprintCallable, Category = "C|Glyph|Base")
	void BaseEvent(EBaseEventType EventType, FBaseEventContext Context);

	UFUNCTION(BlueprintNativeEvent, Category = "C|Glyph|Variant")
	void BaseEventReceived(EBaseEventType EventType, FBaseEventContext Context);

};
