// Fill out your copyright notice in the Description page of Project Settings.


#include "Glyph/C_TravelEventComponent.h"

UC_TravelEventComponent::UC_TravelEventComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UC_TravelEventComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bIsTraveling)return;

	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();
	UC_GlyphBase* Glyph = RunningGlyph.Get();

	if (!Owner || !World || !Glyph) return;

	//发送间隔时间>0时，每隔该时间发送一次事件
	if (SendIntervalTime > 0) {
		if (World->GetTimeSeconds() - LastSendTime >= SendIntervalTime) {
			LastSendTime = World->GetTimeSeconds();
			RunningContext.EventLocation = Owner->GetActorLocation();
			RunningContext.EventToward = Owner->GetActorForwardVector();
			Glyph->BaseEvent(EBaseEventType::OnTravel,RunningContext);
		}
	}

	//发送间隔距离>0时，每隔该距离发送一次事件
	if (SendIntervalDistance > 0) {
		if (FVector::DistSquared(Owner->GetActorLocation(), LastSendLocation) >= SendIntervalDistance * SendIntervalDistance) {
			LastSendLocation = Owner->GetActorLocation();
			RunningContext.EventLocation = Owner->GetActorLocation();
			RunningContext.EventToward = Owner->GetActorForwardVector();
			Glyph->BaseEvent(EBaseEventType::OnTravel, RunningContext);
		}
	}
}

void UC_TravelEventComponent::StartTravelEventSend(UC_GlyphBase* Glyph, float IntervalTime, float IntervalDistance)
{
	if (!IsValid(GetOwner()))return;
	if (!IsValid(GetWorld()))return;

	//初始化相关参数
	RunningGlyph = MakeWeakObjectPtr<UC_GlyphBase>(Glyph);
	RunningContext.BaseGlyph = Glyph;
	SendIntervalTime = IntervalTime;
	SendIntervalDistance = IntervalDistance;
	LastSendTime = GetWorld()->GetTimeSeconds();
	LastSendLocation = GetOwner()->GetActorLocation();

	bIsTraveling = true;
}

void UC_TravelEventComponent::EndTravelEventSend()
{
	bIsTraveling = false;
}

