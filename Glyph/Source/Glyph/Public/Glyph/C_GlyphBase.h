// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "C_GlyphBase.generated.h"

UENUM(BlueprintType)
enum class EGlyphType : uint8
{
	None,
	AttackBase,
	AttackVariant,
	SkillBase,
	SkillVariant,
	MoveBase,
	MoveVariant,
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
	FHitResult HitResult;

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
UCLASS(Blueprintable)
class GLYPH_API UC_GlyphBase : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "C|Glyph")
	FName GlyphName = FName("Base");

	EGlyphType GlyphType = EGlyphType::None;

	UPROPERTY(BlueprintAssignable, Category = "C|Glyph|Event")
	FBaseEventReceived OnBaseEvent;

	UFUNCTION(BlueprintNativeEvent, Category = "C|Glyph|Base")
	void BaseEvent(EBaseEventType EventType, FBaseEventContext Context);

	UFUNCTION(BlueprintNativeEvent, Category = "C|Glyph|Variant")
	void BaseEventReceived(EBaseEventType EventType, FBaseEventContext Context);

};
