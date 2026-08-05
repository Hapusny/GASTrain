// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Glyph/C_GlyphBase.h"
#include "C_TravelEventComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLYPH_API UC_TravelEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_TravelEventComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "C|Travel")
	void StartTravelEventSend(UC_GlyphBase* Glyph,float IntervalTime = 0.f,float IntervalDistance = 0.f);

	UFUNCTION(BlueprintCallable, Category = "C|Travel")
	void EndTravelEventSend();

private:
	bool bIsTraveling = false;

	TWeakObjectPtr<UC_GlyphBase>RunningGlyph = nullptr;

	FGlyphEventContext RunningContext;

	float LastSendTime = 0.f;

	float SendIntervalTime = 0.f;

	FVector LastSendLocation = FVector::ZeroVector;

	float SendIntervalDistance = 0.f;
};
