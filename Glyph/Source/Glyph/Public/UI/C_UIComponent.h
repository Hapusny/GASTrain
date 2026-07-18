// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_UIComponent.generated.h"

class UC_GlyphInventoryWidget;

enum class EGlyphType : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLYPH_API UC_UIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_UIComponent();

	virtual void BeginPlay()override;

	void ShowWidget();

	void CloseWidget();

	UFUNCTION()
	void OnGlyphInventoryChanged(bool bAdded, FName GlyphName);

	UFUNCTION()
	void OnGlyphTypeChanged(EGlyphType NewType, FName GlyphName);

	UPROPERTY(EditDefaultsOnly, Category = "C|UI")
	TSubclassOf<UC_GlyphInventoryWidget>GlyphInventoryWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<UC_GlyphInventoryWidget>GlyphInventoryWidget;
};
