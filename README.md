# 《刻印回廊》

## UE5/GAS · 槽位决定技能 · 非传统技能组合系统

[![UE5](https://img.shields.io/badge/UE-5.3-313131?logo=unrealengine)](https://www.unrealengine.com/)
[![Platform](https://img.shields.io/badge/platform-Windows-informational)]()
[![Status](https://img.shields.io/badge/status-phase%20one%20complete-brightgreen)]()

---

## 演示

<p align="center">
  <a href="https://www.bilibili.com/BV1zzgG6hESy">
    <img src="https://i0.hdslb.com/bfs/archive/634e62fa42e1706039cd663add2826323ac4ca60.jpg@672w_378h_1c.webp" alt="演示视频封面" width="80%"/>
  </a>
</p>

> 演示视频：**【UE5/GAS】槽位决定技能：一套非传统的技能组合系统演示**

---

## 目录

- [项目简介](#项目简介)
- [核心机制](#核心机制)
- [技术架构](#技术架构)
- [快速开始](#快速开始)
- [项目结构](#项目结构)
- [扩展指南](#扩展指南)
- [开发阶段](#开发阶段)
- [相关文档](#相关文档)

---

## 项目简介

**《刻印回廊》** 是一款将技能搭配转化为语法组合的动作 Rogue-like 游戏原型。

核心机制围绕 **刻印（Glyph）** 展开：同一枚刻印放入 **不同功能槽（普攻/技能/位移）** 和 **不同位置（本位/变位）** 会生成完全不同的技能。玩家在 **8 格库容** 限制下，通过位置决定效果的逻辑构建自己的技能体系。

本项目已完成 **第一阶段（刻印系统 MVP）** 的开发，实现了完整的刻印框架、槽位系统、事件通信和蓄力机制。

---

## 核心机制

### 三槽 · 两位置

每个角色拥有三个功能槽，每个功能槽分为两个位置：

| 位置 | 职责 | 说明 |
|------|------|------|
| **本位（槽位一）** | 技能的行为逻辑 | 决定"做什么"，在关键生命周期节点发出事件 |
| **变位（槽位二）** | 技能的修饰行为 | 接收本位事件，检测触发条件后执行响应逻辑 |

### 变位影响本位的三种方式

| 方式 | 占比 | 原理 |
|---|---|---|
| **数据修改** | ~80% | 变位修改 `FGlyphEventContext` 中的数值字段（伤害倍率、投射物数量等），本位只读取数据，完全不知道变位的存在 |
| **事件挂载** | ~15% | 变位监听本位广播的 `OnHit`、`OnCast` 等事件，执行额外逻辑（如命中后挂 DOT、触发爆炸） |
| **管理层覆盖** | ~5% | 由 `UGlyphInventoryComponent` 判断是否由变位接管执行（暂未实现，待真实需求确认后引入） |

### 八枚 MVP 刻印

| 属性 | 刻印 | 普攻本位 | 技能本位 | 位移本位 | 变位效果 |
|---|---|---|---|---|---|
| 火 | 火痕 | 斩击 | 剑气 | 突刺 | 命中挂灼烧 DOT |
| 火 | 燃屑 | 拳击 | 爆炸粉尘 | 翻滚 | 攻击/位移轨迹生成爆炸粉尘 |
| 水 | 露珠 | 法球 | 法球环绕 | 隐身 | 附加蓄力，释放强力版 |
| 水 | 浸润 | 激光 | 回复生命 | 净化 | 伤害转化为水属性 |
| 气 | 轻风 | 飞镖 | 吸引源 | 腾跃 | 动作/移速变快 |
| 气 | 间隙 | 长枪 | 三次突刺 | 瞬身 | 穿透障碍 |
| 土 | 沉砾 | 弓箭 | 范围减速 | 遁地 | 生成减速区域 |
| 土 | 岩片 | 盾牌 | 盾反 | 格挡 | 范围增大，速度减慢 |

---

## 技术架构

### 技术栈

| 模块 | 技术选型 | 分工 |
|------|----------|------|
| 核心框架 | GAS (Gameplay Ability System) | C++ 框架 |
| 刻印系统 | UObject 基类 + 3 个功能槽 Ability | C++ 基类 + 蓝图扩展 |
| 角色逻辑 | 继承 ACharacter | C++ 基类 + 蓝图扩展 |
| 技能效果 | Ability + Effect + Cue | C++ 核心 + 蓝图参数 |
| 动画系统 | 动画蓝图 + 动画通知 | 蓝图状态机 + C++ 接口 |
| UI | UMG | 蓝图布局 + C++ 数据绑定 |

### 核心类关系

```
┌─────────────────────────────────────────────────────────────┐
│                    AC_BaseCharacter                         │
│  ├── UC_GlyphInventoryComponent  ← 刻印库容 & 槽位管理       │
│  └── UC_TravelEventComponent     ← 持续事件广播驱动          │
└─────────────────────────────────────────────────────────────┘
                              │
        ┌─────────────────────┼─────────────────────┐
        ▼                     ▼                     ▼
┌───────────────┐    ┌───────────────┐    ┌───────────────┐
│ Attack Ability│    │  Skill Ability│    │  Move Ability │
│  (功能槽壳)    │    │  (功能槽壳)   │     │  (功能槽壳)   │
└───────────────┘    └───────────────┘    └───────────────┘
        │                     │                     │
        └─────────────────────┼─────────────────────┘
                              ▼
              ┌──────────────────────────────┐
              │   UC_GlyphBase (UObject)     │
              │  ├── AttackConfiguration     │
              │  ├── SkillConfiguration      │
              │  ├── MoveConfiguration       │
              │  ├── BaseEvent() 广播        │
              │  └── BaseEventReceived() 响应│
              └──────────────────────────────┘
```

### 功能槽 Ability —— 薄壳设计

全游戏仅有三个功能槽 `Ability`（Attack/Skill/Move），它们不包含业务逻辑，仅作为 GAS 激活入口：

```cpp
// UC_GameplayAbility::ActivateAbility
void UC_GameplayAbility::ActivateAbility(...)
{
    // 唯一职责：将执行权转交给 InventoryComponent 和刻印
    GIC->ActivateSlotGlyph(SlotType, this);
}
```

所有刻印的差异化逻辑在 `UObject` 蓝图中独立实现。

### 事件通信系统

```cpp
// 本位广播事件
void UC_GlyphBase::BaseEvent(EBaseEventType EventType, const FGlyphEventContext& Context)
{
    OnBaseEvent.Broadcast(EventType, Context);
}

// 变位接收事件（蓝图实现）
UFUNCTION(BlueprintImplementableEvent)
void BaseEventReceived(EBaseEventType EventType, const FGlyphEventContext& Context);
```

**事件类型**：`OnCast` → `OnSpawn` → `OnTravel` → `OnHit` → `OnFinish`

### 关键技术决策

| 决策 | 理由 |
|---|---|
| **刻印继承 UObject 而非 AActor** | 刻印无物理实体，无需 Transform/Component/Tick。UObject 生命周期由 InventoryComponent 管理，无需经由 UWorld 调度 |
| **全游戏仅 3 个 Ability** | 50+ 刻印若每个都创建 Ability 类将产生大量冗余资产。功能槽 Ability 作为薄壳，所有逻辑在刻印 UObject 中实现 |
| **工具函数以基类成员函数提供** | 所有刻印都需要调用碰撞检测、生成物生成等函数，作为基类成员函数调用更简洁，无额外寻址开销 |

---

## 快速开始

### 前置要求

- **引擎版本**：Unreal Engine 5.3（从 Epic Games Launcher 下载）
- **IDE**：Visual Studio 2022 或 Rider（需安装 Unreal Engine 插件）
- **平台**：Windows

### 构建与运行

```bash
# 1. 克隆仓库
git clone https://github.com/Hapusny/GASTrain.git
cd Glyph

# 2. 生成项目文件
右键 Glyph.uproject → Generate Visual Studio project files

# 3. 打开项目
双击 Glyph.uproject

# 4. 编译
在编辑器中点击 Compile，或在 IDE 中构建 Development Editor

# 5. 运行
在 Content Browser 中找到 Map/MainMap 打开，点击 Play
```

### 测试刻印系统

1. 按 `Tab` 键打开库存 UI
2. 从库容区域拖拽刻印到任意功能槽的 **位置一（本位）** 或 **位置二（变位）**
3. 关闭 UI
4. 按 `左键` / `E` / `右键` 分别触发普攻/技能/位移
5. 观察技能效果变化

---

## 项目结构

### C++ 源码目录

```
Source/Glyph/
├── Ability/
│   ├── C_AttributeSet.h/.cpp        # 属性集（含元属性伤害处理）
│   ├── C_GameplayAbility.h/.cpp     # 功能槽 Ability 壳
│   └── C_Tags.h/.cpp                # GameplayTags 定义
├── Character/
│   └── C_BaseCharacter.h/.cpp       # 角色基类
├── Glyph/
│   ├── C_GlyphBase.h/.cpp           # 刻印基类（UObject）
│   ├── C_GlyphInventoryComponent.h/.cpp  # 库容 & 槽位管理
│   ├── C_GlyphSpawnActor.h/.cpp     # 刻印生成物
│   └── C_TravelEventComponent.h/.cpp     # 持续事件组件
├── Player/
│   ├── C_PlayerController.h/.cpp    # 玩家控制器（输入绑定）
│   ├── C_PlayerCharacter.h/.cpp     # 玩家角色
│   └── C_PlayerState.h/.cpp         # 玩家状态（ASC 持有）
├── UI/
│   ├── C_UIComponent.h/.cpp         # UI 组件（数据绑定中转）
│   └── C_GlyphInventoryWidget.h/.cpp    # 库存 UI Widget
└── Enemy/
    ├── C_EnemyStateWidget.h/.cpp    # 敌人状态条 Widget
    ├── C_EnemyCharacter.h/.cpp      # 敌人角色（含权重抽选）
    └── C_EnemyWidgetComponent.h/.cpp    # 敌人血条组件
```

### 蓝图内容目录

```
Content/Game/
├── AbilitySystem/          # GAS 相关资源
│   ├── Ability/            # Ability 蓝图
│   ├── Cue/                # GameplayCue 特效
│   └── Effect/             # GameplayEffect 配置
├── Anim/Notify/            # 动画通知
├── Data/                   # DataTable / DataAsset
├── Enemy/                  # 敌人蓝图
├── Glyph/                  # 刻印蓝图（核心）
├── Input/                  # 增强输入配置
├── Map/                    # 关卡地图
├── Player/                 # 玩家蓝图
└── UI/                     # UMG 界面
```

---

## 扩展指南

### 创建新刻印

1. 在 `Content/Game/Glyph/` 中右键 → **蓝图类** → 继承 `UC_GlyphBase`
2. 命名为 `BP_Glyph_刻印名称`

### 配置刻印参数

在 Details 面板配置三个功能槽的 `FGlyphConfigurationContext`：

| 字段 | 说明 |
|------|------|
| `AttackConfiguration` | 普攻本位配置（伤害、碰撞、生成物、蓄力等） |
| `SkillConfiguration` | 技能本位配置 |
| `MoveConfiguration` | 位移本位配置 |

### 实现本位逻辑

重写以下 `BlueprintImplementableEvent`：

| 事件 | 触发时机 |
|------|----------|
| `AttackBase` | 刻印放入普攻本位时执行 |
| `SkillBase` | 刻印放入技能本位时执行 |
| `MoveBase` | 刻印放入位移本位时执行 |

```cpp
// 示例：火痕-普攻本位（斩击）
void BP_Glyph_Fire::AttackBase()
{
    // 1. 播放蒙太奇
    CreatePlayMontageAndWaitTask("Attack", AttackMontage);
    
    // 2. 碰撞检测（射线检测）
    TArray<AActor*> HitActors = LineCollisionHitCheck(
        GetOwningAbility()->GetAvatarActorFromActorInfo(),
        200.f, 50.f, 0.f, true
    );
    
    // 3. 应用伤害
    for (AActor* Hit : HitActors) {
        ApplyDamageToTarget(Hit, 20.f, EGlyphAttribute::Fire, DamageGE);
    }
    
    // 4. 广播事件（触发变位）
    FGlyphEventContext Context;
    Context.BaseGlyph = this;
    BaseEvent(EBaseEventType::OnCast, Context);
}
```

### 实现变位逻辑

重写 `BaseEventReceived`：

```cpp
// 示例：火痕-变位（命中后挂灼烧 DOT）
void BP_Glyph_Fire::BaseEventReceived(EBaseEventType EventType, const FGlyphEventContext& Context)
{
    if (EventType == EBaseEventType::OnHit && Context.HitActor)
    {
        // 给目标挂燃烧 DOT
        ApplyEffectToTarget(Context.HitActor, FireDOT_GE, 3.f);
    }
}
```

### 工具函数速查

| 函数 | 用途 |
|------|------|
| `CreateWaitGameplayEventTask` | 监听 GAS 事件（如蒙太奇通知） |
| `CreatePlayMontageAndWaitTask` | 播放蒙太奇，自动管理生命周期 |
| `SpawnActor` | 生成刻印生成物（None/Orbit 模式） |
| `SphereCollisionOverlapCheck` | 球体碰撞检测 |
| `BoxCollisionOverlapCheck` | 盒体碰撞检测 |
| `LineCollisionHitCheck` | 射线碰撞检测 |
| `ApplyDamageToTarget` | 即时伤害统一入口 |
| `ApplyEffectToTarget` | 通用效果应用 |

---

## 开发阶段

| 阶段 | 内容 | 状态 |
|---|---|---|
| **阶段一** | 刻印系统 MVP：刻印基类、三槽两位格、事件通信、蓄力系统、8 枚 MVP 刻印 | **已完成** |
| **阶段二** | 战斗循环：角色战斗、敌人 AI、信息素基础功能 | 进行中 |
| **阶段三** | 自适应系统：信息素权重影响、抗性叠加 | 计划中 |
| **阶段四** | 成长与轮替：顿悟融合、残响图鉴、跨局存档 | 计划中 |
| **阶段五** | 内容扩展：50+ 刻印、完整 UI/VFX | 计划中 |

---

## 相关文档

- [游戏设计文档（GDD）](./GDD.docx) — 完整设计细节
- [设计决策记录](./GDD.docx#8.2) — 架构决策与问题修复

---

## 开发者

**Koiro**

---

<p align="center">
  <i>"同一枚刻印放在不同槽位不同位置会生成完全不同的技能"</i>
</p>
