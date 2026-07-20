// Fill out your copyright notice in the Description page of Project Settings.


#include "Glyph/C_GlyphBase.h"

void UC_GlyphBase::BaseEvent_Implementation(EBaseEventType EventType, FBaseEventContext Context)
{
    OnBaseEvent.Broadcast(EventType, Context);
}

void UC_GlyphBase::BaseEventReceived_Implementation(EBaseEventType EventType, FBaseEventContext Context)
{
    
}
