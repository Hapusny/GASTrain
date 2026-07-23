// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/C_UIComponent.h"
#include "UI/C_GlyphInventoryWidget.h"
#include "Player/C_PlayerCharacter.h"
#include "Glyph/C_GlyphInventoryComponent.h"

UC_UIComponent::UC_UIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UC_UIComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GlyphInventoryWidgetClass)return;
	AC_PlayerCharacter* Character = Cast<AC_PlayerCharacter>(GetOwner());
	if (!Character)return;

	//绑定委托
	UC_GlyphInventoryComponent* Inventory = Character->GlyphInventoryComponent;
	if (!Inventory)return;
	Inventory->OnGlyphInventoryChange.AddDynamic(this, &ThisClass::OnGlyphInventoryChanged);
	Inventory->OnGlyphTypeChange.AddDynamic(this, &ThisClass::OnGlyphTypeChanged);

	//创建用户控件
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (!PlayerController)return;
	GlyphInventoryWidget = CreateWidget<UC_GlyphInventoryWidget>(PlayerController, GlyphInventoryWidgetClass);
}

bool UC_UIComponent::GetWidgetState()
{
	if (!GlyphInventoryWidget)return false;
	return GlyphInventoryWidget->IsInViewport();
}

void UC_UIComponent::ShowWidget()
{
	if (!GlyphInventoryWidget)return;
	if(!GlyphInventoryWidget->IsInViewport())GlyphInventoryWidget->AddToViewport();
}

void UC_UIComponent::CloseWidget()
{
	if (!GlyphInventoryWidget)return;
	if(GlyphInventoryWidget->IsInViewport())GlyphInventoryWidget->RemoveFromParent();
}

void UC_UIComponent::OnGlyphInventoryChanged(bool bAdded, FName GlyphName)
{
	if (!GlyphInventoryWidget)return;
	GlyphInventoryWidget->GlyphInventoryChanged(bAdded, GlyphName);
}

void UC_UIComponent::OnGlyphTypeChanged(EGlyphType NewType, FName GlyphName,EGlyphType OldType)
{
	if (!GlyphInventoryWidget)return;
	GlyphInventoryWidget->GlyphTypeChanged(NewType, GlyphName,OldType);
}
