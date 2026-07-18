// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_GlyphInventoryWidget.generated.h"


enum class EGlyphType : uint8;
/**
 * 
 */
UCLASS()
class GLYPH_API UC_GlyphInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION()
	void GlyphInventoryChanged(bool bAdded, FName GlyphName);

	UFUNCTION()
	void GlyphTypeChanged(EGlyphType NewType, FName GlyphName);
};
