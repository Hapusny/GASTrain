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

	TArray<TWeakObjectPtr<UC_GlyphBase>>AttackSlot;

	TArray<TWeakObjectPtr<UC_GlyphBase>>SkillSlot;

	TArray<TWeakObjectPtr<UC_GlyphBase>>MoveSlot;

	//操作0为查询对应类型槽位内容；操作1为清空对应槽位，返回原先内容；操作2为将槽位置为目标刻印，返回原先内容
	UC_GlyphBase* SlotContent(int Operation,EGlyphType Type, UC_GlyphBase* TargetGlyph = nullptr);

	void BindGlyph();

	void UnbindGlyph();
};
