// Fill out your copyright notice in the Description page of Project Settings.


#include "Glyph/C_GlyphSpawnActor.h"
#include "Glyph/C_GlyphBase.h"

AC_GlyphSpawnActor::AC_GlyphSpawnActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AC_GlyphSpawnActor::SetOrbit(float Distance, float AngleSpeed,float Angle)
{
	OrbitDistance = Distance;
	OrbitAngleSpeed = AngleSpeed;
	OrbitAngle = Angle;
}

void AC_GlyphSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SpawnActorType == ESpawnActorType::Orbit) {
		if (!IsValid(GetOwner()))return;
		FVector TargetLocation = FVector(OrbitDistance, 0.f, 0.f).RotateAngleAxis(OrbitAngle, FVector(0.f, 0.f, 1.f)) + GetOwner()->GetActorLocation();
		SetActorLocation(TargetLocation);
		OrbitAngle += OrbitAngleSpeed;
		OrbitAngle = FMath::Wrap(OrbitAngle, 0.f, 360.f);
	}
}



