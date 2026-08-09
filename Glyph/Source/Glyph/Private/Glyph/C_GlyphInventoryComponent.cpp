// Fill out your copyright notice in the Description page of Project Settings.


#include "Glyph/C_GlyphInventoryComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/C_BaseCharacter.h"

UC_GlyphInventoryComponent::UC_GlyphInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	GlyphInventory.Reserve(8);
	AttackSlot.Init(nullptr, 2);
	SkillSlot.Init(nullptr, 2);
	MoveSlot.Init(nullptr, 2);
}

UC_GlyphBase* UC_GlyphInventoryComponent::AddGlyph(TSubclassOf<UC_GlyphBase> GlyphClass)
{
	UC_GlyphBase* NewGlyph = CreateGlyphInstance(GlyphClass);
	if (!NewGlyph)return nullptr;

	//Mvp阶段只有8个刻印，暂不处理超出库存和重复获取的情况

	GlyphInventory.Add((NewGlyph));
	OnGlyphInventoryChange.Broadcast(true, NewGlyph->GlyphName);
	return NewGlyph;
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
	if (VariantGlyph != nullptr) {
		VariantGlyph->OwningAbility = MakeWeakObjectPtr<UGameplayAbility>(Ability);
		RunningContext = VariantGlyph->PreProcessContext(SlotType,RunningContext);
		VariantGlyph->PreActivate();
		FString LogMsg = FString::Printf(TEXT("Base:%s Variant:%s"), *BaseGlyph->GlyphName.ToString(), *VariantGlyph->GlyphName.ToString());
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, LogMsg);
	}
	else {
		FString LogMsg = FString::Printf(TEXT("Base:%s Variant:NULL"), *BaseGlyph->GlyphName.ToString());
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, LogMsg);
	}

	BaseGlyph->OwningAbility = MakeWeakObjectPtr<UGameplayAbility>(Ability);
	BaseGlyph->RunningConfiguration = RunningContext;
	BaseGlyph->PreActivate();


	//蓄力输出模式处理
	if (RunningContext.ChargeContexts.IsEmpty())BaseGlyph->ActivateGlyph();
	else {
		if (!IsValid(GetWorld()))return false;
		RunningGlyph = MakeWeakObjectPtr<UC_GlyphBase>(BaseGlyph);
		ChargeStartTime = GetWorld()->GetTimeSeconds();
		IndexChangeTime = GetWorld()->GetTimeSeconds();
		ChargeIndex = 0;
		for (int i = 0; i < RunningContext.ChargeContexts.Num(); i++)RunningContext.ChargeContexts[i].ChargeTime /= RunningContext.MontageRate;
		if (RunningGlyph.IsValid() && RunningContext.ChargeContexts.Num() > ChargeIndex && IsValid(RunningContext.ChargeContexts[ChargeIndex].ChargeMontage)) {
			RunningMontage = MakeWeakObjectPtr<UAnimMontage>(RunningContext.ChargeContexts[ChargeIndex].ChargeMontage);
			if(RunningMontage.IsValid())PlayChargeMontage(RunningMontage.Get(),RunningContext.MontageRate);
		}
	}
	return true;
}

void UC_GlyphInventoryComponent::EndRunningCharge()
{
	if (!RunningGlyph.IsValid())return;
	if (ChargeIndex != -1 && ChargeIndex < RunningContext.ChargeContexts.Num()) {
		ChargeIndex = -1;
		if (RunningMontage.IsValid())StopChargeMontage(RunningMontage.Get());
		if (!IsValid(GetWorld()))return;
		SettleCharge(RunningContext, GetWorld()->GetTimeSeconds() - ChargeStartTime);
		RunningGlyph->RunningConfiguration = RunningContext;
		RunningGlyph->ActivateGlyph();
	}
}

UC_GlyphBase* UC_GlyphInventoryComponent::CreateGlyphInstance(TSubclassOf<UC_GlyphBase> GlyphClass)
{
	if (!GlyphClass)return nullptr;
	UC_GlyphBase* Glyph = NewObject<UC_GlyphBase>(this, GlyphClass);
	if (!IsValid(Glyph))return nullptr;
	return Glyph;
}

void UC_GlyphInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!IsValid(GetWorld()))return;
	if (ChargeIndex != -1) {
		if (ChargeIndex >= RunningContext.ChargeContexts.Num()) {
			ChargeIndex = -1;
			if(RunningMontage.IsValid())StopChargeMontage(RunningMontage.Get());
			SettleCharge(RunningContext, GetWorld()->GetTimeSeconds() - ChargeStartTime);
			RunningGlyph->RunningConfiguration = RunningContext;
			RunningGlyph->ActivateGlyph();
		}
		else if ((GetWorld()->GetTimeSeconds() - IndexChangeTime) >= RunningContext.ChargeContexts[ChargeIndex].ChargeTime) {
			ChargeIndex++;
			if (RunningGlyph.IsValid() && RunningContext.ChargeContexts.Num() > ChargeIndex && IsValid(RunningContext.ChargeContexts[ChargeIndex].ChargeMontage)) {
				RunningMontage = MakeWeakObjectPtr<UAnimMontage>(RunningContext.ChargeContexts[ChargeIndex].ChargeMontage);
				if (RunningMontage.IsValid())PlayChargeMontage(RunningMontage.Get(), RunningContext.MontageRate);
			}
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
	if (IsValid(Base) && IsValid(Variant))Base->OnBaseEvent.AddUObject(Variant, &UC_GlyphBase::BaseEventReceived);

	Base = SkillSlot[0].Get();
	Variant = SkillSlot[1].Get();
	if (IsValid(Base) && IsValid(Variant))Base->OnBaseEvent.AddUObject(Variant, &UC_GlyphBase::BaseEventReceived);

	Base = MoveSlot[0].Get();
	Variant = MoveSlot[1].Get();
	if (IsValid(Base) && IsValid(Variant))Base->OnBaseEvent.AddUObject(Variant, &UC_GlyphBase::BaseEventReceived);

}

void UC_GlyphInventoryComponent::UnbindGlyph()
{
	UC_GlyphBase* Base, * Variant;
	Base = AttackSlot[0].Get();
	Variant = AttackSlot[1].Get();
	if (IsValid(Base) && IsValid(Variant))Base->OnBaseEvent.RemoveAll(Variant);

	Base = SkillSlot[0].Get();
	Variant = SkillSlot[1].Get();
	if (IsValid(Base) && IsValid(Variant))Base->OnBaseEvent.RemoveAll(Variant);

	Base = MoveSlot[0].Get();
	Variant = MoveSlot[1].Get();
	if (IsValid(Base) && IsValid(Variant))Base->OnBaseEvent.RemoveAll(Variant);
}

void UC_GlyphInventoryComponent::PlayChargeMontage(UAnimMontage* Montage, float Rate)
{
	// 获取角色和动画实例
	AC_BaseCharacter* Owner = Cast<AC_BaseCharacter>(GetOwner());
	if (Owner && Owner->GetMesh())
	{
		UAnimInstance* AnimInst = Owner->GetMesh()->GetAnimInstance();
		if (AnimInst)AnimInst->Montage_Play(Montage, Rate);
	}
}

void UC_GlyphInventoryComponent::StopChargeMontage(UAnimMontage* Montage)
{
	AC_BaseCharacter* Owner = Cast<AC_BaseCharacter>(GetOwner());
	if (Owner && Owner->GetMesh())
	{
		UAnimInstance* AnimInst = Owner->GetMesh()->GetAnimInstance();
		if (AnimInst)AnimInst->Montage_Stop(0.05f,Montage);
	}
}

void UC_GlyphInventoryComponent::SettleCharge(FGlyphConfigurationContext& Context, float ChargeTime)
{
	for (int i = 0; i < Context.ChargeContexts.Num(); i++) {
		if (ChargeTime >= Context.ChargeContexts[i].ChargeTime) {
			//执行对应蓄力修饰
			ApplyChargeRate(Context, i, 1.f);
			ChargeTime -= Context.ChargeContexts[i].ChargeTime;
		}
		else {
			//按比率执行修饰
			ApplyChargeRate(Context, i, ChargeTime / Context.ChargeContexts[i].ChargeTime);
			break;
		}
	}
}

void UC_GlyphInventoryComponent::ApplyChargeRate(FGlyphConfigurationContext& Context, int Index, float TimeRate)
{
	Context.Damage *= (Context.ChargeContexts[Index].DamageRate - 1.f) * TimeRate + 1.f;
	Context.CollisionSize *= (Context.ChargeContexts[Index].CollisionSizeRate - 1.f) * TimeRate + 1.f;
	Context.SpawnActorContext.Damage *= (Context.ChargeContexts[Index].SpawnActorDamageRate - 1.f) * TimeRate + 1.f;
	Context.SpawnActorContext.Size *= (Context.ChargeContexts[Index].SpawnActorSizeRate - 1.f) * TimeRate + 1.f;
	Context.SpawnActorContext.FireSpeed *= (Context.ChargeContexts[Index].SpawnActorSpeedRate - 1.f) * TimeRate + 1.f;
	Context.EffectTime *= (Context.ChargeContexts[Index].EffectTimeRate - 1.f) * TimeRate + 1.f;
	Context.MoveFactor *= (Context.ChargeContexts[Index].MoveFactorRate - 1.f) * TimeRate + 1.f;
}
