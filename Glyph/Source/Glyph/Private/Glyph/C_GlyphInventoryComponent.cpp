// Fill out your copyright notice in the Description page of Project Settings.


#include "Glyph/C_GlyphInventoryComponent.h"
#include "Glyph/C_GlyphBase.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"

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

	UnbindGlyph();

	//将原槽位置空
	SlotContent(1,TargetGlyph->GlyphType);
	OnGlyphTypeChange.Broadcast(EGlyphType::None, TargetGlyph->GlyphName, TargetGlyph->GlyphType);

	BindGlyph();

	if (GlyphInventory.RemoveSingle(TargetGlyph)) {
		OnGlyphInventoryChange.Broadcast(false, TargetGlyph->GlyphName);
	}
}

void UC_GlyphInventoryComponent::SetGlyphType(UC_GlyphBase* TargetGlyph, EGlyphType TargetType)
{
	if (!TargetGlyph || !IsValid(TargetGlyph))return;
	if (TargetType == TargetGlyph->GlyphType)return;

	UnbindGlyph();

	if (UC_GlyphBase* Content = SlotContent(0,TargetType)) {
		SlotContent(2, TargetGlyph->GlyphType,Content);
		OnGlyphTypeChange.Broadcast(TargetGlyph->GlyphType, Content->GlyphName, Content->GlyphType);
		Content->GlyphType = TargetGlyph->GlyphType;
	}
	else {
		//Content为nullptr时不做交换，仅将原槽位置空
		SlotContent(1, TargetGlyph->GlyphType);
		OnGlyphTypeChange.Broadcast(EGlyphType::None, TargetGlyph->GlyphName, TargetGlyph->GlyphType);
	}
	SlotContent(2, TargetType, TargetGlyph);
	OnGlyphTypeChange.Broadcast(TargetType, TargetGlyph->GlyphName, TargetGlyph->GlyphType);
	TargetGlyph->GlyphType = TargetType;

	BindGlyph();
}

void UC_GlyphInventoryComponent::RemoveAllGlyph()
{
	UnbindGlyph();

	for (UC_GlyphBase* Glyph : GlyphInventory){
		OnGlyphInventoryChange.Broadcast(false, Glyph->GlyphName);
		if (Glyph->GlyphType != EGlyphType::None)SlotContent(1,Glyph->GlyphType);
	}

	BindGlyph();
	GlyphInventory.Empty();
}

UC_GlyphBase* UC_GlyphInventoryComponent::NameToGlyph(FName Name)
{
	for (UC_GlyphBase* Glyph : GlyphInventory) {
		if (Glyph->GlyphName == Name)return Glyph;
	}
	return nullptr;
}

bool UC_GlyphInventoryComponent::ActivateSlotGlyph(EGlyphType SlotType, UGameplayAbility* Ability)
{
	if (SlotType != EGlyphType::AttackBase && SlotType != EGlyphType::SkillBase && SlotType != EGlyphType::MoveBase) return false;
	UC_GlyphBase* TargetGlyph = SlotContent(0, SlotType);
	if (!IsValid(TargetGlyph))return false;
	TargetGlyph->ActivateGlyph(Ability);
	return true;
}

UC_GlyphBase* UC_GlyphInventoryComponent::GetBaseSlotGlyph(UC_GlyphBase* Variant)
{
	switch (Variant->GlyphType)
	{
	case EGlyphType::AttackVariant:
		if (AttackSlot[0].IsValid())return AttackSlot[0].Get();
	case EGlyphType::SkillVariant:
		if (SkillSlot[0].IsValid())return SkillSlot[0].Get();
	case EGlyphType::MoveVariant:
		if (MoveSlot[0].IsValid())return MoveSlot[0].Get();
	default:break;
	}
	return nullptr;
}


UC_GlyphBase* UC_GlyphInventoryComponent::CreateGlyphInstance(TSubclassOf<UC_GlyphBase> GlyphClass)
{
	if (!GlyphClass)return nullptr;
	return NewObject<UC_GlyphBase>(this, GlyphClass);
}

UC_GlyphBase* UC_GlyphInventoryComponent::SlotContent(int Operation, EGlyphType Type, UC_GlyphBase* TargetGlyph)
{
	UC_GlyphBase* Glyph = nullptr;
	switch (Type) {
	case EGlyphType::AttackBase:
		if(AttackSlot[0].IsValid())Glyph = AttackSlot[0].Get();
		if(Operation == 1)AttackSlot[0] = nullptr;
		if(Operation == 2)AttackSlot[0] = TWeakObjectPtr<UC_GlyphBase>(TargetGlyph);
		break;
	case EGlyphType::AttackVariant:
		if(AttackSlot[1].IsValid())Glyph = AttackSlot[1].Get();
		if (Operation == 1)AttackSlot[1] = nullptr;
		if (Operation == 2)AttackSlot[1] = TWeakObjectPtr<UC_GlyphBase>(TargetGlyph);
		break;
	case EGlyphType::SkillBase:
		if(SkillSlot[0].IsValid())Glyph = SkillSlot[0].Get();
		if (Operation == 1)SkillSlot[0] = nullptr;
		if (Operation == 2)SkillSlot[0] = TWeakObjectPtr<UC_GlyphBase>(TargetGlyph);
		break;
	case EGlyphType::SkillVariant:
		if(SkillSlot[1].IsValid())Glyph = SkillSlot[1].Get();
		if (Operation == 1)SkillSlot[1] = nullptr;
		if (Operation == 2)SkillSlot[1] = TWeakObjectPtr<UC_GlyphBase>(TargetGlyph);
		break;
	case EGlyphType::MoveBase:
		if(MoveSlot[0].IsValid())Glyph = MoveSlot[0].Get();
		if (Operation == 1)MoveSlot[0] = nullptr;
		if (Operation == 2)MoveSlot[0] = TWeakObjectPtr<UC_GlyphBase>(TargetGlyph);
		break;
	case EGlyphType::MoveVariant:
		if(MoveSlot[1].IsValid())Glyph = MoveSlot[1].Get();
		if (Operation == 1)MoveSlot[1] = nullptr;
		if (Operation == 2)MoveSlot[1] = TWeakObjectPtr<UC_GlyphBase>(TargetGlyph);
		break;
	default:break;
	}
	return Glyph;
}

void UC_GlyphInventoryComponent::BindGlyph()
{
	UC_GlyphBase* Base, *Variant;
	Base = AttackSlot[0].Get();
	Variant = AttackSlot[1].Get();
	if (IsValid(Base) && IsValid(Variant))Base->OnBaseEvent.AddDynamic(Variant, &UC_GlyphBase::BaseEventReceived);

	Base = SkillSlot[0].Get();
	Variant = SkillSlot[1].Get();
	if (IsValid(Base) && IsValid(Variant))Base->OnBaseEvent.AddDynamic(Variant, &UC_GlyphBase::BaseEventReceived);

	Base = MoveSlot[0].Get();
	Variant = MoveSlot[1].Get();
	if (IsValid(Base) && IsValid(Variant))Base->OnBaseEvent.AddDynamic(Variant, &UC_GlyphBase::BaseEventReceived);

}

void UC_GlyphInventoryComponent::UnbindGlyph()
{
	UC_GlyphBase* Base, * Variant;
	Base = AttackSlot[0].Get();
	Variant = AttackSlot[1].Get();
	if (IsValid(Base) && IsValid(Variant))Base->OnBaseEvent.RemoveDynamic(Variant, &UC_GlyphBase::BaseEventReceived);

	Base = SkillSlot[0].Get();
	Variant = SkillSlot[1].Get();
	if (IsValid(Base) && IsValid(Variant))Base->OnBaseEvent.RemoveDynamic(Variant, &UC_GlyphBase::BaseEventReceived);

	Base = MoveSlot[0].Get();
	Variant = MoveSlot[1].Get();
	if (IsValid(Base) && IsValid(Variant))Base->OnBaseEvent.RemoveDynamic(Variant, &UC_GlyphBase::BaseEventReceived);
}