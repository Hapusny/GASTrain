// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_GlyphInventoryComponent.generated.h"

class UC_GlyphBase;

enum class EGlyphType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGlyphInventoryChange,bool,bIsAdd,FName,GlyphName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGlyphTypeChange, EGlyphType,NewGlyphType , FName, GlyphName);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLYPH_API UC_GlyphInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_GlyphInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "C|GlyphInventory")
	void AddGlyph(TSubclassOf<UC_GlyphBase> GlyphClass);

	UFUNCTION(BlueprintCallable, Category = "C|GlyphInventory")
	void RemoveGlyph(UC_GlyphBase* TargetGlyph);

	UFUNCTION(BlueprintCallable, Category = "C|GlyphInventory")
	void SetGlyphType(UC_GlyphBase* TargetGlyph,EGlyphType TargetType);

	UFUNCTION(BlueprintCallable, Category = "C|GlyphInventory")
	void RemoveAllGlyph();

	UPROPERTY(BlueprintAssignable, Category = "C|GlyphInventory")
	FGlyphInventoryChange OnGlyphInventoryChange;

	UPROPERTY(BlueprintAssignable, Category = "C|GlyphInventory")
	FGlyphTypeChange OnGlyphTypeChange;

protected:
	UC_GlyphBase* CreateGlyphInstance(TSubclassOf<UC_GlyphBase> GlyphClass);

private:
	UPROPERTY()
	TArray<TObjectPtr<UC_GlyphBase>>GlyphInventory;

	UPROPERTY()
	TArray<TWeakObjectPtr<UC_GlyphBase>>AttackSlot;

	UPROPERTY()
	TArray<TWeakObjectPtr<UC_GlyphBase>>SkillSlot;

	UPROPERTY()
	TArray<TWeakObjectPtr<UC_GlyphBase>>MoveSlot;

	UC_GlyphBase* GetSlotContent(EGlyphType Type);

	void SetSlotContent(UC_GlyphBase* TargetGlyph, EGlyphType TargetType);

	void ClearSlotContent(EGlyphType Type);

};
