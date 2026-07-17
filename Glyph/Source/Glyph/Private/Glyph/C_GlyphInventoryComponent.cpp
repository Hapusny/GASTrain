// Fill out your copyright notice in the Description page of Project Settings.


#include "Glyph/C_GlyphInventoryComponent.h"
#include "Glyph/C_GlyphBase.h"

UC_GlyphInventoryComponent::UC_GlyphInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	GlyphInventory.Reserve(8);
	AttackSlot.Init(nullptr, 2);
	SkillSlot.Init(nullptr, 2);
	MoveSlot.Init(nullptr, 2);
}

void UC_GlyphInventoryComponent::AddGlyph(TSubclassOf<UC_GlyphBase> GlyphClass)
{
	UC_GlyphBase* NewGlyph = CreateGlyphInstance(GlyphClass);
	if (!NewGlyph)return;
	GlyphInventory.Add((NewGlyph));
	OnGlyphInventoryChange.Broadcast(true, NewGlyph->GlyphName);
}

void UC_GlyphInventoryComponent::RemoveGlyph(UC_GlyphBase* TargetGlyph)
{
	if (!TargetGlyph || !IsValid(TargetGlyph))return;
	ClearSlotContent(TargetGlyph->GlyphType);
	if (GlyphInventory.RemoveSingle(TargetGlyph)) {
		OnGlyphInventoryChange.Broadcast(false, TargetGlyph->GlyphName);
	}
}

void UC_GlyphInventoryComponent::SetGlyphType(UC_GlyphBase* TargetGlyph, EGlyphType TargetType)
{
	if (!TargetGlyph || !IsValid(TargetGlyph))return;
	if (TargetType == TargetGlyph->GlyphType)return;
	if (UC_GlyphBase* Content = GetSlotContent(TargetType)) {
		SetSlotContent(Content, TargetGlyph->GlyphType);
		Content->GlyphType = TargetGlyph->GlyphType;
		OnGlyphTypeChange.Broadcast(TargetGlyph->GlyphType, Content->GlyphName);
	}
	else ClearSlotContent(TargetGlyph->GlyphType);//Content为nullptr时不做交换，仅将原槽位置空
	SetSlotContent(TargetGlyph, TargetType);
	TargetGlyph->GlyphType = TargetType;
	OnGlyphTypeChange.Broadcast(TargetType, TargetGlyph->GlyphName);
}

void UC_GlyphInventoryComponent::RemoveAllGlyph()
{
	for (UC_GlyphBase* Glyph : GlyphInventory){
		OnGlyphInventoryChange.Broadcast(false, Glyph->GlyphName);
		if (Glyph->GlyphType != EGlyphType::None)ClearSlotContent(Glyph->GlyphType);
	}
	GlyphInventory.Empty();
}

UC_GlyphBase* UC_GlyphInventoryComponent::CreateGlyphInstance(TSubclassOf<UC_GlyphBase> GlyphClass)
{
	if (!GlyphClass)return nullptr;
	return NewObject<UC_GlyphBase>(this, GlyphClass);
}

UC_GlyphBase* UC_GlyphInventoryComponent::GetSlotContent(EGlyphType Type)
{
	UC_GlyphBase* Glyph = nullptr;
	switch (Type) {
	case EGlyphType::AttackBase:
		if(AttackSlot[0].IsValid())Glyph = AttackSlot[0].Get();
		break;
	case EGlyphType::AttackVariant:
		if (AttackSlot[1].IsValid())Glyph = AttackSlot[1].Get();
		break;
	case EGlyphType::SkillBase:
		if (SkillSlot[0].IsValid())Glyph = SkillSlot[0].Get();
		break;
	case EGlyphType::SkillVariant:
		if (SkillSlot[1].IsValid())Glyph = SkillSlot[1].Get();
		break;
	case EGlyphType::MoveBase:
		if (MoveSlot[0].IsValid())Glyph = MoveSlot[0].Get();
		break;
	case EGlyphType::MoveVariant:
		if (MoveSlot[1].IsValid())Glyph = MoveSlot[1].Get();
		break;
	default:break;
	}
	return Glyph;
}

void UC_GlyphInventoryComponent::SetSlotContent(UC_GlyphBase* TargetGlyph, EGlyphType TargetType)
{
	if (!TargetGlyph)return;
	switch (TargetType) {
	case EGlyphType::AttackBase:
		AttackSlot[0] = TWeakObjectPtr<UC_GlyphBase>(TargetGlyph);
		break;
	case EGlyphType::AttackVariant:
		AttackSlot[1] = TWeakObjectPtr<UC_GlyphBase>(TargetGlyph);
		break;
	case EGlyphType::SkillBase:
		SkillSlot[0] = TWeakObjectPtr<UC_GlyphBase>(TargetGlyph);
		break;
	case EGlyphType::SkillVariant:
		SkillSlot[1] = TWeakObjectPtr<UC_GlyphBase>(TargetGlyph);
		break;
	case EGlyphType::MoveBase:
		MoveSlot[0] = TWeakObjectPtr<UC_GlyphBase>(TargetGlyph);
		break;
	case EGlyphType::MoveVariant:
		MoveSlot[1] = TWeakObjectPtr<UC_GlyphBase>(TargetGlyph);
		break;
	default:break;
	}
}

void UC_GlyphInventoryComponent::ClearSlotContent(EGlyphType Type)
{
	switch (Type) {
	case EGlyphType::AttackBase:
		if (AttackSlot[0].IsValid())AttackSlot[0] = nullptr;
		break;
	case EGlyphType::AttackVariant:
		if (AttackSlot[1].IsValid())AttackSlot[1] = nullptr;
		break;
	case EGlyphType::SkillBase:
		if (SkillSlot[0].IsValid())SkillSlot[0] = nullptr;
		break;
	case EGlyphType::SkillVariant:
		if (SkillSlot[1].IsValid())SkillSlot[1] = nullptr;
		break;
	case EGlyphType::MoveBase:
		if (MoveSlot[0].IsValid())MoveSlot[0] = nullptr;
		break;
	case EGlyphType::MoveVariant:
		if (MoveSlot[1].IsValid())MoveSlot[1] = nullptr;
		break;
	default:break;
	}
}

