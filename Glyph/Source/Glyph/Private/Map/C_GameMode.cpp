// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/C_GameMode.h"
#include "Enemy//C_EnemyCharacter.h"

AC_EnemyCharacter* AC_GameMode::SpawnEnemy(TSubclassOf<AC_EnemyCharacter> ActorClass, FVector Location, TArray<float> Values)
{
    UWorld* World = GetWorld();
    AC_EnemyCharacter* Actor = nullptr;
    if (!IsValid(World))return Actor;

    if (!IsValid(ActorClass))return Actor;
    Actor = World->SpawnActor<AC_EnemyCharacter>(ActorClass, Location,FRotator(0.f,0.f,0.f));
    if (!IsValid(Actor))return Actor;
    if (Values.Num() == 0) return Actor;

    // 1. 计算总权重
    float TotalWeight = 0.0f;
    for (float Weight : Values)
    {
        TotalWeight += Weight;
    }

    // 2. 生成随机数 [0, TotalWeight)
    float RandomValue = FMath::FRandRange(0.0f, TotalWeight);

    // 3. 遍历找到对应索引
    float Cumulative = 0.0f;
    for (int32 i = 0; i < Values.Num(); i++)
    {
        Cumulative += Values[i];
        if (RandomValue < Cumulative)
        {
            Actor->GiveGlyph(i + 1);
            return Actor;
        }
    }

    // 保底返回最后一个（浮点误差兜底）
    Actor->GiveGlyph(Values.Num());
    return Actor;
}
