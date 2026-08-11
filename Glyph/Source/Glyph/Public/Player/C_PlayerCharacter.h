// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/C_BaseCharacter.h"
#include "C_PlayerCharacter.generated.h"

class UC_UIComponent;
class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class GLYPH_API AC_PlayerCharacter : public AC_BaseCharacter
{
	GENERATED_BODY()
	
public:
	AC_PlayerCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;

	virtual UAttributeSet* GetAttributeSet()const override;


	virtual void BeginPlay()override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UC_UIComponent>UIComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera");
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera");
	TObjectPtr<UCameraComponent> FollowCamera;
};
