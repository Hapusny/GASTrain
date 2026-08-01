// Fill out your copyright notice in the Description page of Project Settings.


#include "Glyph/C_GlyphInventoryComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"

UC_GlyphInventoryComponent::UC_GlyphInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

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
	//获取对应槽位刻印
	UC_GlyphBase* BaseGlyph = nullptr, *VariantGlyph = nullptr;
	switch (SlotType)
	{
	case EGlyphType::AttackBase:
		if (AttackSlot[0].IsValid())BaseGlyph = AttackSlot[0].Get();
		if (AttackSlot[1].IsValid())VariantGlyph = AttackSlot[1].Get();
		if (BaseGlyph == nullptr)return false;
		RunningContext = BaseGlyph->AttackConfiguration;
		break;
	case EGlyphType::SkillBase:
		if (SkillSlot[0].IsValid())BaseGlyph = SkillSlot[0].Get();
		if (SkillSlot[1].IsValid())VariantGlyph = SkillSlot[1].Get();
		if (BaseGlyph == nullptr)return false;
		RunningContext = BaseGlyph->SkillConfiguration;
		break;
	case EGlyphType::MoveBase:
		if (MoveSlot[0].IsValid())BaseGlyph = MoveSlot[0].Get();
		if (MoveSlot[1].IsValid())VariantGlyph = MoveSlot[1].Get();
		if (BaseGlyph == nullptr)return false;
		RunningContext = BaseGlyph->MoveConfiguration;
		break;
	default:
		return false;
	}

	//预处理
	BaseGlyph->PreActivate();

	if (VariantGlyph != nullptr) {
		VariantGlyph->OwningAbility = MakeWeakObjectPtr<UGameplayAbility>(Ability);
		RunningContext = VariantGlyph->PreProcessContext(SlotType,RunningContext);
		VariantGlyph->PreActivate();
	}

	//蓄力输出模式处理
	RunningAbility = MakeWeakObjectPtr<UGameplayAbility>(Ability);
	if (RunningContext.ChargeTime.IsEmpty())BaseGlyph->ActivateGlyph(RunningAbility.Get(), RunningContext);
	else {
		if (!IsValid(GetWorld()))return false;
		RunningGlyph = MakeWeakObjectPtr<UC_GlyphBase>(BaseGlyph);
		ChargeStartTime = GetWorld()->GetTimeSeconds();
		IndexChangeTime = GetWorld()->GetTimeSeconds();
		ChargeIndex = 0;
	}
	return true;
}

void UC_GlyphInventoryComponent::EndRunningCharge()
{
	if (!RunningAbility.IsValid() || !RunningGlyph.IsValid())return;
	if (ChargeIndex != -1 && ChargeIndex < RunningContext.ChargeTime.Num()) {
		ChargeIndex = -1;
		if (!IsValid(GetWorld()))return;
		SettleCharge(RunningContext, GetWorld()->GetTimeSeconds() - ChargeStartTime);
		RunningGlyph->ActivateGlyph(RunningAbility.Get(), RunningContext);
	}
}

UC_GlyphBase* UC_GlyphInventoryComponent::CreateGlyphInstance(TSubclassOf<UC_GlyphBase> GlyphClass)
{
	if (!GlyphClass)return nullptr;
	return NewObject<UC_GlyphBase>(this, GlyphClass);
}

void UC_GlyphInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!IsValid(GetWorld()))return;
	if (ChargeIndex != -1) {
		if (ChargeIndex >= RunningContext.ChargeTime.Num()) {
			ChargeIndex = -1;
			SettleCharge(RunningContext, GetWorld()->GetTimeSeconds() - ChargeStartTime);
			if (RunningAbility.IsValid())RunningGlyph->ActivateGlyph(RunningAbility.Get(), RunningContext);
		}
		else if ((GetWorld()->GetTimeSeconds() - IndexChangeTime) >= RunningContext.ChargeTime[ChargeIndex]) {
			ChargeIndex++;
			IndexChangeTime = GetWorld()->GetTimeSeconds();
		}
	}
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

void UC_GlyphInventoryComponent::SettleCharge(FBaseGlyphContext& Context, float ChargeTime)
{
	for (int i = 0; i < Context.ChargeTime.Num(); i++) {
		if (ChargeTime >= Context.ChargeTime[i]) {
			//执行对应蓄力修饰

			ChargeTime -= Context.ChargeTime[i];
		}
		else {
			//按比率执行修饰
		}
	}
}
