// Fill out your copyright notice in the Description page of Project Settings.


#include "Glyph/C_GlyphBase.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
void UC_GlyphBase::BaseEvent_Implementation(EBaseEventType EventType, FBaseEventContext Context)
{
    OnBaseEvent.Broadcast(EventType, Context);
}

void UC_GlyphBase::BaseEventReceived_Implementation(EBaseEventType EventType, FBaseEventContext Context)
{
    
}

void UC_GlyphBase::ActivateGlyph_Implementation(UGameplayAbility* Ability)
{

}