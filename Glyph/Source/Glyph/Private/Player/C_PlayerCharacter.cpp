// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/C_PlayerCharacter.h"
#include "Player/C_PlayerState.h"
#include "UI/C_UIComponent.h"
#include "Glyph/C_GlyphInventoryComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h"

AC_PlayerCharacter::AC_PlayerCharacter()
{
	UIComponent = CreateDefaultSubobject<UC_UIComponent>("UIComponent");

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

UAbilitySystemComponent* AC_PlayerCharacter::GetAbilitySystemComponent() const
{
	if (!IsValid(GetPlayerState<AC_PlayerState>()))return nullptr;
	return GetPlayerState<AC_PlayerState>()->GetAbilitySystemComponent();
}

UAttributeSet* AC_PlayerCharacter::GetAttributeSet() const
{
	if (!IsValid(GetPlayerState<AC_PlayerState>()))return nullptr;
	return GetPlayerState<AC_PlayerState>()->GetAttributeSet();
}

void AC_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	GiveStartupAbilities();
}
