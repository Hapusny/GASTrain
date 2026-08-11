// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/C_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "Glyph/C_GlyphInventoryComponent.h"
#include "Ability/C_Tags.h"
#include "Player/C_PlayerCharacter.h"
#include "UI/C_UIComponent.h"

void AC_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(InputSubsystem))return;

	for (UInputMappingContext* Context : InputMappingContexts) {
		InputSubsystem->AddMappingContext(Context, 0);
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent))return;

	EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ThisClass::Walk);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::Attack);
	EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &ThisClass::Skill);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ThisClass::EndCharge);
	EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Completed, this, &ThisClass::EndCharge);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::EndCharge);
	EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this, &ThisClass::Menu);
}

void AC_PlayerController::Walk(const FInputActionValue& Value)
{
	if (!IsValid(GetPawn()))return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetPawn()->AddMovementInput(RightDirection, MovementVector.X);
}

void AC_PlayerController::Look(const FInputActionValue& Value)
{
	if (bLookingMenu)return;
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void AC_PlayerController::Attack()
{
	ActivateAbility(CTags::Abilities::Attack);
}

void AC_PlayerController::Skill()
{
	ActivateAbility(CTags::Abilities::Skill);
}

void AC_PlayerController::Move()
{
	ActivateAbility(CTags::Abilities::Move);
}

void AC_PlayerController::Menu()
{
	AC_PlayerCharacter* PlayerCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (!IsValid(PlayerCharacter))return;
	UC_UIComponent* UIComponent = PlayerCharacter->UIComponent;
	if (!IsValid(UIComponent))return;

	//控件显示中则隐藏，反之显示
	if (UIComponent->GetWidgetState()) {
		UIComponent->CloseWidget();
		SetShowMouseCursor(false);
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		bLookingMenu = false;
	}
	else {
		UIComponent->ShowWidget();
		SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
		bLookingMenu = true;
	}
}

void AC_PlayerController::EndCharge()
{
	AC_PlayerCharacter* PlayerCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (!IsValid(PlayerCharacter))return;
	UC_GlyphInventoryComponent* GIC = PlayerCharacter->GlyphInventoryComponent;
	if (!IsValid(GIC))return;
	GIC->EndRunningCharge();
}

void AC_PlayerController::ActivateAbility(const FGameplayTag& AbilityTag) const
{
	if (bLookingMenu)return;
	AC_PlayerCharacter* PlayerCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (!IsValid(PlayerCharacter))return;
	UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(ASC))return;
	ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
}
