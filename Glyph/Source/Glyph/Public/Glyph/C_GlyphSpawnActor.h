// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_GlyphSpawnActor.generated.h"

class UC_GlyphBase;

UENUM(BlueprintType)
enum class ESpawnActorType : uint8
{
	None,
	Orbit
};

UCLASS()
class GLYPH_API AC_GlyphSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AC_GlyphSpawnActor();

	UPROPERTY(BlueprintReadWrite, Category = "C|Glyph")
	TWeakObjectPtr<UC_GlyphBase>Glyph;

	UPROPERTY(BlueprintReadOnly, Category = "C|Actor")
	ESpawnActorType SpawnActorType = ESpawnActorType::None;

	void SetOrbit(float Distance, float AngleSpeed,float Angle);

protected:
	virtual void Tick(float DeltaTime) override;

private:
	float OrbitDistance = 100.f;
	float OrbitAngleSpeed = 1.f;
	float OrbitAngle = 0.f;
};
