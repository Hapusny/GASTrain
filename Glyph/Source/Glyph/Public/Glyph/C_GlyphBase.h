// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "C_GlyphBase.generated.h"

class UGameplayAbility;
class UAbilityTask;
class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;


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

UENUM(BlueprintType)
enum class EGlyphSpawnActorType : uint8
{
	None,
	RapidFire,
	Scatter,
	Orbit
};

USTRUCT(BlueprintType)
struct FGlyphSpawnActorContext
{
	GENERATED_BODY()

	//基础伤害 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	float Damage = 0.f;

	//伤害属性 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	EGlyphAttribute DamageAttribute = EGlyphAttribute::None;

	//尺寸
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	FVector Size = FVector(1.f, 1.f, 1.f);

	//前方偏移 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	float ForwardOffset = 0.f;

	//上方偏移 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	float ElevationOffset = 0.f;

	//是否重写旋转 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	bool bOverrideRotation = false;

	//旋转（当 bOverrideRotation 为 true 时使用） 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	FQuat Rotation = FQuat::Identity;

	//发射速度 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	float FireSpeed = 0.f;

	//是否可穿透 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	bool bCanPenetrate = false;

	//是否重写原点 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	bool bOverrideOrigin = false;

	//原点（当 bOverrideOrigin 为 true 时使用） 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	FVector Origin = FVector::ZeroVector;

	//生成物类型 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	EGlyphSpawnActorType SpawnType = EGlyphSpawnActorType::None;

	//生成数量（散射/环绕时有效）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	int32 Number = 1;

	//连射时长（连射时有效）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	float ContinuousTime = 0.f;

	//散射角度（散射时有效，单位：度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	float ScatteringAngle = 0.f;

	//环绕距离（环绕时有效）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	float OrbitDistance = 100.f;

	//环绕速度（环绕时有效，单位：度） 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|SpawnActor")
	float OrbitAngleSpeed = 0.f;
};

USTRUCT(BlueprintType)
struct FGlyphChargeContext
{
	GENERATED_BODY()

	//蓄力时间（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Charge")
	float ChargeTime = 0.f;

	//蓄力蒙太奇 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Charge")
	UAnimMontage* ChargeMontage = nullptr;

	//伤害影响倍率 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Charge")
	float DamageRate = 1.f;

	//碰撞尺寸影响倍率 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Charge")
	float CollisionSizeRate = 1.f;

	//生成物伤害影响倍率 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Charge")
	float SpawnActorDamageRate = 1.f;

	//生成物尺寸影响倍率 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Charge")
	float SpawnActorSizeRate = 1.f;

	//生成物速度影响倍率 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Charge")
	float SpawnActorSpeedRate = 1.f;

	//效果持续时间影响倍率 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Charge")
	float EffectTimeRate = 1.f;

	//位移系数影响倍率 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Charge")
	float MoveFactorRate = 1.f;
};

USTRUCT(BlueprintType)
struct FGlyphConfigurationContext
{
	GENERATED_BODY()

	//基础伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Config")
	float Damage = 0.f;

	//伤害属性 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Config")
	EGlyphAttribute DamageAttribute = EGlyphAttribute::None;

	//碰撞尺寸 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Config")
	FVector CollisionSize = FVector::ZeroVector;

	//碰撞前方偏移 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Config")
	float HitBoxForwardOffset = 0.f;

	//碰撞上方偏移 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Config")
	float HitBoxElevationOffset = 0.f;

	//动作速率倍率 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Config")
	float MontageRate = 1.f;

	//生成物上下文 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Config")
	FGlyphSpawnActorContext SpawnActorContext;

	//蓄力上下文数组（支持多段蓄力） 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Config")
	TArray<FGlyphChargeContext> ChargeContexts;

	//效果持续时间 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Config")
	float EffectTime = 0.f;

	//位移系数 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glyph|Config")
	float MoveFactor = 0.f;
};

USTRUCT(BlueprintType)
struct FGlyphEventContext
{
	GENERATED_BODY()

	//本位刻印引用（用于获取刻印信息及使用刻印函数） 
	UPROPERTY(BlueprintReadWrite, Category = "Glyph|Event")
	UC_GlyphBase* BaseGlyph = nullptr;

	//事件发生位置 
	UPROPERTY(BlueprintReadWrite, Category = "Glyph|Event")
	FVector EventLocation = FVector::ZeroVector;

	//事件发生朝向 
	UPROPERTY(BlueprintReadWrite, Category = "Glyph|Event")
	FVector EventToward = FVector::ZeroVector;

	//碰撞目标
	UPROPERTY(BlueprintReadWrite, Category = "Glyph|Event")
	AActor* HitActor;
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FBaseEventReceived, EBaseEventType, const FGlyphEventContext&);

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

	//Glyph Configuration
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C|Glyph|Attack")
	FGlyphConfigurationContext AttackConfiguration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C|Glyph|Skill")
	FGlyphConfigurationContext SkillConfiguration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C|Glyph|Move")
	FGlyphConfigurationContext MoveConfiguration;

	UPROPERTY(BlueprintReadOnly, Category = "C|Glyph")
	FGlyphConfigurationContext RunningConfiguration;

	//Tool Function
	UFUNCTION(BlueprintCallable, Category = "C|Tool|Tasks")
	UAbilityTask_WaitGameplayEvent* CreateWaitGameplayEventTask(FGameplayTag EventTag, AActor* OptionalExternalTarget = nullptr, bool OnlyTriggerOnce = false, bool OnlyMatchExact = true);

	UFUNCTION(BlueprintCallable, Category = "C|Tool|Tasks")
	UAbilityTask_PlayMontageAndWait* CreatePlayMontageAndWaitTask(FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f, bool bAllowInterruptAfterBlendOut = false);

	UFUNCTION(BlueprintCallable, Category = "C|Tool|Spawn")
	TArray<AActor*> SpawnActor(TSubclassOf<AC_GlyphSpawnActor> ActorClass,const FGlyphSpawnActorContext& GlyphSpawnActorContext);

	UFUNCTION(BlueprintCallable, Category = "C|Tool|Collision")
	TArray<AActor*> SphereCollisionOverlapCheck(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset = 0.f, float HitBoxElevationOffset = 0.f, bool bDrawDebugs = false);

	UFUNCTION(BlueprintCallable, Category = "C|Tool|Collision")
	TArray<AActor*> BoxCollisionOverlapCheck(AActor* AvatarActor, FVector BoxSize, float HitBoxForwardOffset = 0.f, float HitBoxElevationOffset = 0.f, bool bDrawDebugs = false);

	UFUNCTION(BlueprintCallable, Category = "C|Tool|Collision")
	TArray<AActor*> LineCollisionHitCheck(AActor* AvatarActor, float Length, float HitBoxForwardOffset = 0.f, float HitBoxElevationOffset = 0.f, bool bDrawDebugs = false);

	//Glyph Ability
	UFUNCTION(BlueprintNativeEvent, Category = "C|Glyph|Variant")
	FGlyphConfigurationContext PreProcessContext(EGlyphType BaseGlyphType, FGlyphConfigurationContext Context);

	UFUNCTION(BlueprintImplementableEvent, Category = "C|Glyph")
	void PreActivate();

	void ActivateGlyph(const FGlyphConfigurationContext& Context);

	UFUNCTION(BlueprintImplementableEvent, Category = "C|Glyph|Base")
	void AttackBase();

	UFUNCTION(BlueprintImplementableEvent, Category = "C|Glyph|Base")
	void SkillBase();

	UFUNCTION(BlueprintImplementableEvent, Category = "C|Glyph|Base")
	void MoveBase();

	FBaseEventReceived OnBaseEvent;

	UFUNCTION(BlueprintCallable, Category = "C|Glyph|Base")
	void BaseEvent(EBaseEventType EventType, const FGlyphEventContext& Context);

	UFUNCTION(BlueprintImplementableEvent, Category = "C|Glyph|Variant")
	void BaseEventReceived(EBaseEventType EventType, const FGlyphEventContext& Context);

	UFUNCTION(BlueprintCallable, Category = "C|Glyph")
	void TryEndAbility();

private:
	void InitializeActorByContext(AC_GlyphSpawnActor* Actor,const FGlyphSpawnActorContext& Context);
};
