# 《刻印回廊》游戏设计文档 (GDD)

引擎版本: Unreal Engine 5.3  
核心框架: GAS (Gameplay Ability System)  
项目类型: 3D 动作 Rogue-like  
开发者: Koiro  
版本: v2.1（2026-11-28，依据当前实现与项目整理修订）

---

## 目录

- [1. 游戏概述](#1-游戏概述)
  - [1.1. 概念](#11-概念)
  - [1.2. 核心目标](#12-核心目标)
  - [1.3. 设计目标](#13-设计目标)
  - [1.4. 技术栈概览](#14-技术栈概览)
- [2. 核心玩法](#2-核心玩法)
  - [2.1. 核心循环](#21-核心循环)
  - [2.2. 双段波次结构](#22-双段波次结构)
  - [2.3. 刻印系统](#23-刻印系统)
    - [2.3.1. 刻印基础概念](#231-刻印基础概念)
    - [2.3.2. 槽位架构](#232-槽位架构)
    - [2.3.3. 库容管理](#233-库容管理)
    - [2.3.4. 顿悟融合机制](#234-顿悟融合机制)
    - [2.3.5. 词条系统](#235-词条系统)
    - [2.3.6. 槽位配置与瞬切](#236-槽位配置与瞬切)
    - [2.3.7. 残响图鉴](#237-残响图鉴)
  - [2.4. 敌人系统](#24-敌人系统)
    - [2.4.1. 小怪](#241-小怪)
    - [2.4.2. 刻印怪](#242-刻印怪)
    - [2.4.3. 信息素机制](#243-信息素机制)
    - [2.4.4. 抗性机制](#244-抗性机制)
  - [2.5. 基础战斗属性](#25-基础战斗属性)
  - [2.6. 视角与操作](#26-视角与操作)
  - [2.7. 玩家体验节奏](#27-玩家体验节奏)
- [3. 刻印系统设计](#3-刻印系统设计)
  - [3.1. 刻印结构](#31-刻印结构)
  - [3.2. 蓄力系统](#32-蓄力系统)
  - [3.3. 事件系统](#33-事件系统)
  - [3.4. 工具函数](#34-工具函数)
    - [3.4.1. AbilityTask 创建函数](#341-abilitytask-创建函数)
    - [3.4.2. 生成物生成函数](#342-生成物生成函数)
    - [3.4.3. 碰撞检测函数](#343-碰撞检测函数)
    - [3.4.4. 伤害与效果应用函数](#344-伤害与效果应用函数)
  - [3.5. 刻印制作流程](#35-刻印制作流程)
- [4. AI 系统设计](#4-ai-系统设计)
  - [4.1. 黑板设计](#41-黑板设计)
  - [4.2. 行为树结构](#42-行为树结构)
  - [4.3. BTTask 设计](#43-bttask-设计)
    - [4.3.1. 统一执行模式](#431-统一执行模式)
    - [4.3.2. 位移类任务](#432-位移类任务)
    - [4.3.3. 技能类任务](#433-技能类任务)
    - [4.3.4. 普攻类任务](#434-普攻类任务)
  - [4.4. BTService 设计](#44-btservice-设计)
  - [4.5. 装饰器设计](#45-装饰器设计)
- [5. 环境与表现系统](#5-环境与表现系统)
  - [5.1. 场景结构](#51-场景结构)
  - [5.2. 环境异化系统](#52-环境异化系统)
  - [5.3. 视觉反馈规范](#53-视觉反馈规范)
- [6. 技术架构](#6-技术架构)
  - [6.1. 项目结构](#61-项目结构)
  - [6.2. C++/蓝图分工](#62-c蓝图分工)
  - [6.3. 核心类设计](#63-核心类设计)
    - [6.3.1. UGlyphInventoryComponent（库容管理）](#631-uglyphinventorycomponent库容管理)
    - [6.3.2. UGlyphBase（刻印基类）](#632-uglyphbase刻印基类)
    - [6.3.3. UTravelEventComponent（持续事件转发组件）](#633-utraveleventcomponent持续事件转发组件)
    - [6.3.4. UAttributeSet（属性集）](#634-uattributeset属性集)
    - [6.3.5. 敌人类拆分](#635-敌人类拆分)
    - [6.3.6. 融合刻印 UC_FusedGlyph](#636-融合刻印-uc_fusedglyph)
    - [6.3.7. 信息素账户 FPheromoneData / AC_GameState](#637-信息素账户-fpheromonedata--ac_gamestate)
  - [6.4. 数据驱动配置](#64-数据驱动配置)
    - [6.4.1. 刻印生成物上下文 FGlyphSpawnActorContext](#641-刻印生成物上下文-fglyphspawnactorcontext)
    - [6.4.2. 刻印蓄力上下文 FGlyphChargeContext](#642-刻印蓄力上下文-fglyphchargecontext)
    - [6.4.3. 刻印配置上下文 FGlyphConfigurationContext](#643-刻印配置上下文-fglyphconfigurationcontext)
    - [6.4.4. 刻印事件上下文 FGlyphEventContext](#644-刻印事件上下文-fglypheventcontext)
    - [6.4.5. 词条 FGlyphAffix](#645-词条-fglyphaffix)
    - [6.4.6. 信息素 FPheromoneData](#646-信息素-fpheromonedata)
    - [6.4.7. 槽位预存 FSlotPreset](#647-槽位预存-fslotpreset)
    - [6.4.8. 数据表与数据资产分工](#648-数据表与数据资产分工)
  - [6.5. UI 架构](#65-ui-架构)
- [7. 开发计划](#7-开发计划)
  - [7.1. 开发里程碑](#71-开发里程碑)
  - [7.2. 刻印清单与实现阶段](#72-刻印清单与实现阶段)
  - [7.3. Demo 范围](#73-demo-范围)
  - [7.4. 当前实现状态](#74-当前实现状态)
- [8. 验收清单](#8-验收清单)
  - [8.1. 功能验收](#81-功能验收)
  - [8.2. 技术验收点](#82-技术验收点)
- [9. 附录](#9-附录)
  - [9.1. 术语表](#91-术语表)
  - [9.2. 设计决策记录](#92-设计决策记录)
    - [9.2.1. 刻印设计](#921-刻印设计)
    - [9.2.2. 刻印基类使用 UObject 而非 AActor](#922-刻印基类使用-uobject-而非-aactor)
    - [9.2.3. 工具函数以基类成员函数提供而非独立组件](#923-工具函数以基类成员函数提供而非独立组件)
    - [9.2.4. 功能槽 Ability 作为执行壳](#924-功能槽-ability-作为执行壳)
    - [9.2.5. AbilityTask C++ 中转方案](#925-abilitytask-c-中转方案)
    - [9.2.6. 变位影响本位的配置预处理与事件机制](#926-变位影响本位的配置预处理与事件机制)
    - [9.2.7. 蓄力系统设计](#927-蓄力系统设计)
    - [9.2.8. 元属性伤害处理架构](#928-元属性伤害处理架构)
    - [9.2.9. 小怪不套用三槽架构](#929-小怪不套用三槽架构)
    - [9.2.10. 顿悟不可攒、触发即用](#9210-顿悟不可攒触发即用)
    - [9.2.11. 本版技术方案决策（2026-11-28）](#9211-本版技术方案决策2026-11-28)
  - [9.3. 问题修复记录](#93-问题修复记录)
    - [9.3.1. AbilityTask 激活与清理问题](#931-abilitytask-激活与清理问题)
    - [9.3.2. 第一次播放蓄力蒙太奇无动画](#932-第一次播放蓄力蒙太奇无动画)
  - [9.4. 待确认事项](#94-待确认事项)

---

## 1. 游戏概述

### 1.1. 概念

将技能搭配转化为语法组合的动作 Rogue-like。同一枚刻印放在不同槽位不同位置会生成完全不同的技能，玩家在 8 格库容限制下通过位置决定效果的逻辑构筑自己的技能体系，以应对配置会动态适应玩家策略的敌人。

战斗分为双段：小怪波以俯视角割草积累信息素与刻印词条，Boss 波以第三人称越肩对抗由信息素喂养出的刻印怪，击败后顿悟强化构筑，坠落更深一层。"玩家喂什么，就打什么"——信息素喂养与抗性趋同机制迫使玩家在不同套路之间切换。

### 1.2. 核心目标

跑通"刻印组合→技能生成→战斗→信息素反馈→刻印怪生成→顿悟强化"全链路；八格有限库容配合三槽两位格与融合机制，在容量限制下提供丰富的构筑选择；信息素驱动刻印怪配置随玩家行为变化，每局对战逻辑不同；残响图鉴解锁初始池选项，使每局游戏有积累感。

### 1.3. 设计目标

- **高策略深度**：八格库容配合三槽两位格的位格系统及融合机制，组合空间极大。
- **动态重复可玩性**：信息素喂养驱动刻印怪配置变化，每局对战逻辑不同。
- **自平衡生态**：信息素喂养与抗性趋同双重机制，玩家喂什么就打什么；库容广度是抗性对策，逼玩家保持类型覆盖，防止单一套路通吃。
- **跨局成长**：残响图鉴解锁初始池选项，每局有积累感。
- **刷宝与语法共存**：词条随深度成长提供即时回报，刻印语法保证构筑独特性。

### 1.4. 技术栈概览

| 模块 | 技术选型 | C++/蓝图分工 |
|---|---|---|
| 核心框架 | GAS (Gameplay Ability System) | C++框架 |
| 角色逻辑 | 继承自 ACharacter | C++基类 + 蓝图扩展 |
| 刻印系统 | UObject 基类 + 3 个功能槽 Ability | C++基类 + 3 个 Ability 入口 + 蓝图 |
| 技能效果 | Ability + Effect + Cue | C++核心 + 蓝图参数配置 |
| 词条系统 | 数据结构 + SaveGame | C++数据结构 + 蓝图配置 |
| 信息素系统 | 数据结构 + 权重抽取 | C++核心 |
| 双视角系统 | 相机组件 + 输入模式切换 | C++框架 + 蓝图配置 |
| 动画系统 | 动画蓝图 + 动画通知 | 蓝图状态机 + C++接口 |
| UI | UMG | 蓝图布局 + C++数据绑定 |
| 存档 | SaveGame | C++序列化 |

---

## 2. 核心玩法

### 2.1. 核心循环

```
开局选择初始刻印池
  → 小怪波（俯视角割草，击杀累积信息素与刻印词条，概率爆刻印，概率触发顿悟）
  → 小怪波清剿完成
  → Boss 波（越肩视角，吸收本层全部信息素并归零，生成刻印怪，对抗）
  → 击败 Boss 必触发顿悟（融合 / 升级 / 跳过）
  → 坠落更深一层
  → 每五层触发环境异象
  → 死亡进入下一轮回，残响图鉴解锁新初始池选项
```

### 2.2. 双段波次结构

每层战斗分为两个阶段：

**小怪波（俯视角）**
- 玩家以俯视角进行割草式战斗，普攻自动或半自动，技能与位移手动。
- 小怪为经过设计的固定怪物，拥有固定行为与外貌，不套用玩家三槽架构。
- 击杀小怪时累积信息素，按击杀所用技能的元素属性（Fire / Water / Wind / Soil）记录。
- 击杀小怪有概率爆出刻印，刻印由随机语法与随机词条组成。
- 击杀小怪有概率触发顿悟。
- 小怪波清剿完成后触发 Boss 波。

**Boss 波（第三人称越肩）**
- 切换为第三人称越肩视角，三槽全手动。
- Boss 波开始时吸收本层全部信息素并归零，作为生成刻印怪的喂养材料。
- 刻印怪复用玩家完整的三槽架构，配置由刻印池随机生成，向玩家信息素偏好趋同。
- 刻印怪抗性按被吸收信息素中该伤害类型的占比决定，占比越高抗性越高。
- 外观以发光颜色体现信息素与抗性，体型体现刻印数与大类型，具体刻印在战斗中揭示。
- 击败 Boss 必触发顿悟。
- 支持"概率多只稍弱"（初版实现单只，多只标记扩展）。

**转场**
- Boss 吸收信息素的动画同时完成空间重构、相机过渡、界面与输入模式切换，作为双视角转场，时长 3~5 秒，可跳过。

### 2.3. 刻印系统

#### 2.3.1. 刻印基础概念

刻印是构筑的基础单元。每枚刻印是一个独立的蓝图类，同时包含本位和变位两套行为逻辑。本位（Base）是技能的核心行为骨架，界定"做什么"，在关键生命周期节点发出事件。每枚刻印为三个功能槽（普攻/技能/位移）各设计一套本位逻辑，分别对应不同的释放方式和数值强度。变位（Variant）是技能的修饰行为模块，在激活前修改本位的运行配置，或接收本位发出的事件执行增量逻辑。

刻印由"语法 + 随机词条"组成：语法决定本位与变位的行为范式，词条为数值附魔（详见 2.3.5）。

变位对本位的修饰采用“激活前配置修改 + 事件挂载增量逻辑”两层机制：

- **激活前配置修改（主要方式）**：功能槽激活时，InventoryComponent 先取本位刻印的 `FGlyphConfigurationContext` 作为 `RunningConfiguration`，再调用变位的 `PreProcessContext(SlotType, Context)` 修改这份运行配置（伤害、碰撞尺寸、生成物数量/速度、效果持续时间、位移系数、蓄力段参数等），最后才执行本位。本位只读取运行配置，不知道变位存在。
- **激活前准备**：变位可通过 `PreActivate` 在正式执行前做状态准备（如建立临时效果、监听蒙太奇事件）。
- **事件挂载增量逻辑（次要方式）**：变位通过 `BaseEventReceived(EventType, Context)` 监听本位广播的 `OnCast / OnSpawn / OnTravel / OnHit / OnFinish` 事件，执行额外逻辑（如命中后追加 DOT、生成爆炸、播放特效），不修改事件上下文，也不修改本位已经读取的数值。
- **管理层覆盖（暂不实现）**：极端情况下由 InventoryComponent 判断是否由变位接管执行；当前阶段不实现，待有真实需求再引入。

变位逻辑只做一套，通用于三个功能槽；变位刻印的 `GlyphType`（AttackVariant / SkillVariant / MoveVariant）标识其所在功能槽，事件响应与预处理时据此分支。

核心机制为“配置预处理 + 事件驱动”：本位在生命周期关键节点广播事件，变位监听并响应；本位不持有变位引用，也不为变位预留逻辑分支，两者通过运行配置与事件通信解耦。

设计原则：每枚刻印的本位逻辑和变位逻辑写在同一个蓝图中；每枚刻印独立设计，不复用整枚刻印的逻辑，只复用工具函数。

#### 2.3.2. 槽位架构

角色拥有三个功能槽，每个功能槽由两个位置组成。槽位一放置的刻印表现本位效果，槽位二放置的刻印表现变位效果。若某位置为空，则使用默认基础能力（普攻为近战挥击，技能为空，位移为翻滚）。

三个功能槽各自承担不同的战斗职能。普攻槽为基础输出循环，伤害较低、冷却短或无冷却。技能槽为高收益爆发技能，伤害高、冷却长，可能附带额外效果。位移槽为生存或机动技能，以位移为主体，可能附带伤害或效果。位移槽决定基础移动方式。

同一枚刻印放入不同功能槽时执行不同的本位逻辑。变位逻辑不区分功能槽，统一修饰当前槽位的本位效果。例如炎弹的变位效果为"命中后生成燃烧区域"，无论刻印放在普攻槽、技能槽还是位移槽，都会在命中或位移结束时生成燃烧区域。

同一枚刻印本位与变位互换会产生本质相异的技能表现，放在不同功能槽效果也不同。

实现说明：三个功能槽 Ability（Attack/Skill/Move）是游戏仅有的 GameplayAbility 类。它们不包含业务逻辑，仅作为 GAS 的激活入口，将执行权转交给 InventoryComponent 和刻印。所有刻印的差异化逻辑在 UObject 蓝图中独立实现，通过 AbilityRef 调用 GAS 功能。

#### 2.3.3. 库容管理

库容上限恒定为 8 格，只存不同刻印。同一时刻，库容中只能存在同一刻印类（如 BP_Glyph_Fire）的一个实例。

**获取刻印**：击杀小怪有概率爆出刻印，刻印由随机语法与随机词条组成。库满时获取新刻印须永久舍弃一枚现存刻印。

**重复获取（整枚二选一）**：重复获取同一刻印不触发升级，而是触发整枚二选一——保留旧刻印（当前等级与旧词条）或换新刻印（新刻印固定为 1 级，携带新词条）。被放弃的刻印转化为残响录入图鉴。

**等级晋升**：等级晋升只能通过顿悟，上限三级。刻印等级只影响技能倍率与表现，不改变词条。新获取的刻印从 1 级开始；融合刻印继承主源等级。

**残响来源**：舍弃、替换以及二选一中被放弃的刻印均转化为残响录入图鉴。残响是重复获取的固定收获，代表对该刻印的印象。

应对刻印怪抗性的核心是库容的广度——八格尽量覆盖不同伤害类型与行为范式，使抗性趋同某类型时仍有其他类型可用。爆装时的取舍、重复获取的整枚二选一、顿悟的融合与升级，都是库容管理的决策。

#### 2.3.4. 顿悟融合机制

顿悟不可攒，触发即用，可跳过。击杀小怪有约 5% 概率触发（可调）；击败 Boss 必触发。

顿悟时三选一：

- **融合**：选择两枚同级刻印，指定其中一枚为主源，融合为一枚融合刻印。融合刻印占用 1 个库容格，不可再融合或升级。
  - **行为构成**：融合刻印 = 主源的本位逻辑 + 副源的变位逻辑。主源决定三个功能槽的本位行为与等级；副源只贡献一套变位逻辑，其本位逻辑不保留；主源的变位逻辑与副源的词条不保留。
  - **主源选择**：由融合界面选择，默认第一枚选中的刻印为主源（具体交互在 UI 阶段确定）。
  - **等级**：融合刻印等级 = 主源等级；不可再升级。
- **升级**：将一枚未满级、未融合的刻印提升一级（作为重复获取之外的唯一升级途径，上限三级）。
- **跳过**：放弃本次顿悟。

融合的价值在于将两枚刻印的功能压缩进一枚的空间，释放库容和槽位，实现构筑的纵向突破。

#### 2.3.5. 词条系统

刻印附带随机词条，词条为数值附魔，与刻印等级分离，随层数分段成长，越深爆出的词条数值越高。Demo 阶段每枚刻印 1 条词条，词条池仅属性类。

- 刻印等级只影响技能倍率与表现，不改变词条。
- 词条池仅属性类（如伤害加成、冷却缩减、范围增加等）。
- 词条与语法共同构成刻印：语法保证构筑独特性，词条提供即时回报。
- 词条由“词条类型 + 数值 + 层级区间”组成；词条池配置在数据表 `DT_Affix` 中，刻印实例用 `FGlyphAffix` 保存抽取结果。
- 词条不进入 AttributeSet；在生成 `RunningConfiguration` 时并入数值。具体应用顺序见 9.4 待确认事项。

#### 2.3.6. 槽位配置与瞬切

槽位配置指三槽两位格放哪些刻印、怎么摆。刻印来自当前库容，同一套库容可摆出不同技能组合。

- 玩家可预存一套槽位配置，按键瞬间切换；预存配置保存 6 个放置位对当前库容刻印实例的引用。
- 切换前校验引用刻印仍在库容、未被融合或舍弃；校验失败则拒绝切换并保留当前配置。
- 瞬切冷却 5 秒（可调）。
- 初始一套，后续可提升（扩展规划，初版仅实现一套）。
- 槽位与库容可随时调整，全战斗可用，但无时停，玩家须自行确保安全。
- 开库为半透明侧边栏，可见战场、可移动、不可攻击。
- 波间怪物生成动画与层间坠落动画是更安全的调整窗口。

#### 2.3.7. 残响图鉴

舍弃、替换以及二选一中被放弃的刻印均转化为残响录入图鉴。残响是重复获取的固定收获，代表对该刻印的印象。

跨局成长：残响图鉴解锁初始池选项，使每局游戏有积累感。存档通过 SaveGame 序列化实现。

### 2.4. 敌人系统

#### 2.4.1. 小怪

小怪是经过设计的固定怪物，拥有固定行为与外貌，不套用玩家三槽架构，不参与标签决策查询。实现上使用独立的小怪类 `AC_SmallMobCharacter`，与刻印怪共享敌人基类，但不持有 `UC_GlyphInventoryComponent`。

- 小怪波以俯视角进行割草式战斗。
- 击杀小怪时累积信息素，按击杀所用刻印的元素属性记入本层账户。
- 击杀小怪有概率爆出刻印（随机语法 + 随机词条）。
- 击杀小怪有约 5% 概率触发顿悟（可调）。
- 小怪波清剿完成后触发 Boss 波。

#### 2.4.2. 刻印怪

刻印怪是 Boss 波特有的敌人，复用玩家完整的三槽架构（普攻/技能/位移），配置由刻印池随机生成。实现上使用独立的刻印怪类 `AC_GlyphMonsterCharacter`，与固定小怪共享敌人基类，持有完整的 `UC_GlyphInventoryComponent`。

- 配置向玩家信息素偏好趋同：含相同元素标签的刻印获得三倍抽取权重。
- 抗性按被吸收信息素中该伤害类型的占比决定，占比越高抗性越高。
- 外观以发光颜色体现信息素与抗性；体型体现刻印数与大类型；具体刻印在战斗中揭示。
- 初版单只；支持"概率多只稍弱"作为后续扩展。

**体型与功能槽规则**：

| 体型 | 功能槽位数 | 解锁规则 | 生命倍率 |
|---|---|---|---|
| 小型 | 1~2 | 普攻必开，技能 50% 概率 | ×1.0 |
| 中型 | 2~3 | 普攻+技能必开，位移 50% 概率 | ×1.6 |
| 大型 | 2~3 | 普攻+技能必开，位移 50% 概率 | ×2.8 |
| 巨型 | 3 | 三槽全开 | ×4.5 |

功能槽解锁优先级：普攻 > 技能 > 位移，保证任意敌人都能攻击。

**变位槽（分阶段启用）**：

| 层数区间 | 变位解锁状态 |
|---|---|
| 1~4 层 | 全部敌人无变位 |
| 5~9 层 | 大型/巨型敌人开启 1 个变位 |
| 10~14 层 | 全部敌人开启 1 个变位 |
| 15~19 层 | 大型/巨型敌人开启 2 个变位 |
| 20 层+ | 全部敌人开启 2 个变位 |

> 实施节奏：M1/M2 暂不启用敌人变位槽；M3 起先启用简化规则“1~4 层无、5 层起大型/巨型各 1 个”，上表作为后续完整目标。

变位槽作为功能槽的附加层，不单独占用槽位计数。具体敌人实际变位数 = min(体型允许上限，当前进度允许上限)。空变位槽不产生修饰效果。变位刻印从当前层级的变位池中独立随机抽取，不受信息素影响。

**时机驱动的分层 AI 决策**：刻印怪 AI 采用固定行为树骨架 + 时机驱动的标签查询，不采用权重混算。仅本位标签参与行为决策，变位标签被忽略。决策分为三层，按执行时机分层查询：

- **位移决策**（刚被赋予 / 受到威胁时触发）：读取位移槽刻印的本位标签，Dash 向玩家方向突进，Escape 远离玩家（向后/侧向位移），无标签/空槽不主动使用位移。
- **技能决策**（技能槽冷却完毕时触发）：读取技能槽刻印的本位标签，Ranged 保持 600~1000 距离释放，Melee 移动到近战范围（<200）释放，AOE 移动到玩家当前位置释放，Heal 生命 <50% 时释放，Summon 在玩家周围选点召唤，无标签默认近战范围释放。
- **普攻决策**（技能槽冷却中时触发）：读取普攻槽刻印的本位标签，Ranged 保持 600~1000 距离循环普攻，Melee 近战范围循环普攻，无标签/空槽 → 默认近战挥击。

**行为标签定义**：

| 标签 | 含义 | 影响阈值 | 适用槽位 |
|---|---|---|---|
| Melee | 近战攻击 | 距离 <200 | 普攻/技能 |
| Ranged | 远程攻击 | 保持 600~1000 | 普攻/技能 |
| AOE | 范围技能 | 移动到玩家位置释放 | 技能 |
| Dash | 突进 | 向玩家突进 400~600 | 位移 |
| Escape | 躲避 | 远离玩家 500~800 | 位移 |
| Heal | 治疗 | 生命 <50% 时释放 | 技能 |
| Summon | 召唤 | 在玩家周围召唤 | 技能 |

设计原则：行为可预期。玩家通过 UI 看到敌人携带的刻印标签，即可预判其行动模式，形成"观察、对策、执行"的策略循环。变位标签不参与决策，保证行为逻辑纯净。

标签口径：元素保留 `Wind` / `Soil`（与代码、标签一致）；行为标签以本章七类为准。现有 `Tags.AI.Range` 迁移为 `Ranged`，`Tags.AI.Effect` 按刻印实际语义迁移为 `AOE` / `Heal` / `Summon`，具体映射随内容迁移确认。

#### 2.4.3. 信息素机制

小怪波中击杀小怪时，按击杀所用刻印的 `GlyphAttribute`（Fire / Water / Wind / Soil）把信息素累积到本层账户。`None` 不计入。信息素账户由 `AC_GameState` 持有（`FPheromoneData`），`AC_Pheromone` 仅作为表现层，不作为数据源。

Boss 波开始时吸收本层全部信息素并归零，作为生成刻印怪的喂养材料。

关键链路：信息素按元素累积 → 刻印怪抽取权重偏向同元素刻印 → 刻印怪携带同元素刻印 → 分层 AI 读取标签 → 行为自动适配标签语义。

跨层规则：Boss 吸收后本层账户归零；每层从零开始累积，不再额外执行“每五层全局重置”。

玩家喂什么，就打什么，这套机制迫使玩家在不同套路之间切换。

#### 2.4.4. 抗性机制

刻印怪抗性按被吸收信息素中该伤害类型的占比决定，占比越高抗性越高。初版公式：

`该元素抗性 = 该元素信息素量 / 本层信息素总量 × 60%`（总量为 0 时抗性为 0），抗性上限 60%，数值可调。

刻印怪生成时把四种元素抗性写入 `UC_AttributeSet`，伤害管线按现有抗性逻辑结算。

外观以发光颜色体现信息素与抗性（红→紫渐变等视觉警示）。跨层不额外重置：Boss 吸收后本层账户归零，下一层从零累积。

### 2.5. 基础战斗属性

- 生命值 100
- 移动速度 400
- 库容固定 8 格
- 词条数值随层数成长
- 灵感值已废弃：顿悟改为概率/必触发，不保留内部计数与 UI

### 2.6. 视角与操作

- **小怪波**：俯视角，普攻自动或半自动，技能与位移手动。
- **Boss 波**：第三人称越肩，三槽全手动。
- **转场**：Boss 吸收信息素的动画同时完成空间重构、相机过渡、界面与输入模式切换，3~5 秒可跳过。
- **位移槽**决定基础移动方式。
- 调整与瞬切不暂停游戏，全程风险自负。

### 2.7. 玩家体验节奏

- **轮回 1~2**：理解位置决定效果的语法，三槽八格等级 1~2，小怪波割草与爆装建立爽感。
- **轮回 3~5**：验证套路构建，感受信息素喂养与 Boss 波对抗，等级 3、顿悟、融合、残响图鉴全面展开。
- **轮回 6 及以上**：融合构筑深度，应对动态变化的刻印怪。后续扩展规划包括体系选择、第四槽、宝物及进阶刻印等。

---

## 3. 刻印系统设计

### 3.1. 刻印结构

每枚刻印是一个独立的蓝图类，继承自刻印基类 UGlyphBase（UObject 子类）。基础信息包括唯一标识符 `GlyphID`、显示名称、描述文本、图标资源、最大等级/当前等级、词条数据、元素属性 `GlyphAttribute` 与当前放置类型 `GlyphType`。

刻印本身无物理实体，不继承 AActor。所有刻印实例由 UGlyphInventoryComponent 持有和管理，通过 `NewObject<UC_GlyphBase>()` 创建。

**本位入口**：三个功能槽分别对应 `AttackBase` / `SkillBase` / `MoveBase`（BlueprintImplementableEvent）。`ActivateGlyph` 按 `GlyphType` 分发到对应入口；刻印在本位逻辑中通过 AbilityRef 调用 GAS 功能并完成核心行为。

**生命周期事件**：`OnCast` / `OnSpawn` / `OnTravel` / `OnHit` / `OnFinish`。这些事件由本位在关键节点主动调用 `BaseEvent(EventType, Context)` 广播，不是基类自动调用的 BlueprintNativeEvent。

| 事件 | 触发时机 | 广播方式 |
|---|---|---|
| OnCast() | 技能释放后 | 本位蓝图调用 BaseEvent |
| OnSpawn() | 生成实体后 | 本位蓝图调用 BaseEvent |
| OnTravel() | 途径过程中 | UTravelEventComponent 按时间/距离驱动 BaseEvent |
| OnHit() | 命中目标后 | 本位蓝图或 ApplyDamageToTarget 调用 BaseEvent |
| OnFinish() | 技能结束前 | 本位蓝图调用 BaseEvent |

**事件上下文 FGlyphEventContext 只读**：包含 `BaseGlyph`、`EventLocation`、`EventToward`、`HitActor`，仅用于通知与增量逻辑；变位不修改事件上下文。需要修饰数值时，在激活前通过 `PreProcessContext` 修改 `FGlyphConfigurationContext`。

**变位接入**：`PreProcessContext`（修改运行配置）、`PreActivate`（激活前准备）、`BaseEventReceived`（事件增量）。基类中 `BaseEvent` 负责广播，`BaseEventReceived` 由变位蓝图实现。变位刻印的 `GlyphType`（AttackVariant / SkillVariant / MoveVariant）表示其所在功能槽。

**AbilityRef 传递机制**：功能槽 Ability 激活时，将自身引用（AbilityRef）传递给 InventoryComponent，后者将其转发给刻印的执行函数。刻印通过 AbilityRef 调用 GAS 原生功能，通过 `IsValid()` 检测 Ability 生命周期。本位不持有变位引用，也不为变位预留逻辑分支。

### 3.2. 蓄力系统

蓄力作为刻印的可选行为，由 UGlyphInventoryComponent 统一调度，与 GAS 的 AbilityTask 解耦。刻印的 `FGlyphConfigurationContext` 中包含 `TArray<FGlyphChargeContext> ChargeContexts`，每段包含蓄力时间、蓄力蒙太奇与七项倍率（伤害、碰撞尺寸、生成物伤害/尺寸/速度、效果持续时间、位移系数）。

`ActivateSlotGlyph` 在功能槽 Ability 被激活时由 InventoryComponent 调用，是蓄力流程的统一入口。当 `ChargeContexts` 为空时，刻印不蓄力，直接调用 `ActivateGlyph`；非空时启动蓄力流程，`TickComponent` 以绝对时间差逐段推进 `ChargeIndex`，在每段达到时播放对应的蓄力蒙太奇。当前实现不提供独立的 `OnChargeLevelReached` 事件；如后续需要逐段通知，再在基类补充。

蓄力中断或完成时调用 `SettleCharge`，按实际蓄力总时长把各段倍率应用到 `RunningConfiguration`，然后执行 `ActivateGlyph`。

蓄力动画不通过 `AbilityTask_PlayMontageAndWait` 播放，而是由 InventoryComponent 通过 `AnimInstance->Montage_Play` 直接播放，避免打包后首次播放时的异步资源时序问题（详见 9.3.2）；蒙太奇中的 `UAnimNotify_SendGameplayEvent` 仍通过 `CreateWaitGameplayEventTask`（详见 9.2.5）供刻印监听蓄力过程事件。

**Ability 生命周期守卫**：Ability 结束/取消时蓄力中断，停止蒙太奇，不执行激活；蓄力完成时仅在 `OwningAbility` 仍有效的情况下调用 `ActivateGlyph`。蓄力状态仍由 InventoryComponent 独立管理，不依赖 AbilityTask 生命周期。

### 3.3. 事件系统

事件类型共有五种：Cast（技能释放）、Spawn（生成实体）、Travel（持续过程）、Hit（命中目标）、Finish（技能结束）。事件上下文 `FGlyphEventContext` 为只读通知数据，包含：

- `BaseGlyph`：本位刻印引用；
- `EventLocation`：事件位置；
- `EventToward`：事件方向；
- `HitActor`：命中目标。

事件不承载“可修改伤害/扩展参数”的职责；变位需要修改数值时，在激活前通过 `PreProcessContext` 修改运行配置，而不是在事件响应中修改 Context。

事件流：功能槽 Ability 被激活后调用 `UC_GlyphInventoryComponent::ActivateSlotGlyph(SlotType, AbilityRef)`。InventoryComponent 取该槽位的本位与变位，先用变位的 `PreProcessContext` 修改运行配置，再调用本位的 `PreActivate` / `ActivateGlyph`。本位在执行过程中通过 `BaseEvent(EventType, Context)` 广播事件；变位在 `BaseEventReceived` 中接收，按事件类型与自身 `GlyphType` 执行增量逻辑（如命中后追加 DOT、生成爆炸、播放特效），不修改事件上下文。

变位不直接调用本位的内部函数；本位不持有变位引用。功能槽类型由变位刻印自身的 `GlyphType`（AttackVariant / SkillVariant / MoveVariant）表示。OnTravel 的持续广播由 `UC_TravelEventComponent` 驱动，该组件挂载于角色或刻印生成物，以时间间隔或移动距离间隔触发 `BaseEvent(OnTravel, Context)`。

### 3.4. 工具函数

刻印基类 UC_GlyphBase 直接提供一系列工具函数，供所有刻印在蓝图和 C++ 中调用。通用能力通过基类函数提供，而非拆分为独立组件。

#### 3.4.1. AbilityTask 创建函数

| 函数 | 参数 | 说明 |
|---|---|---|
| CreateWaitGameplayEventTask | EventTag: 事件标签；OptionalExternalTarget: 可选外部目标；OnlyTriggerOnce: 是否仅触发一次；OnlyMatchExact: 是否精确匹配 | 创建 UAbilityTask_WaitGameplayEvent 实例，用于监听 GAS 事件（如蒙太奇通知触发）。内部调用工厂函数创建 Task 后立即调用 ReadyForActivation() 激活，返回 Task 指针供刻印绑定回调。 |
| CreatePlayMontageAndWaitTask | TaskInstanceName: 任务实例名；MontageToPlay: 蒙太奇资源；Rate: 播放速率；StartSection: 起始片段；bStopWhenAbilityEnds: 技能结束时是否停止；AnimRootMotionTranslationScale: RootMotion 缩放；StartTimeSeconds: 起始时间；bAllowInterruptAfterBlendOut: 混合结束后是否允许中断 | 创建 UAbilityTask_PlayMontageAndWait 实例，内部调用工厂函数创建 Task 后立即调用 ReadyForActivation() 激活，并自动绑定 OnBlendOut、OnCancelled、OnCompleted、OnInterrupted 回调至 TryEndAbility，在动画结束后自动尝试结束功能槽 Ability。 |

#### 3.4.2. 生成物生成函数

| 函数 | 参数 | 说明 |
|---|---|---|
| SpawnActor | ActorClass: 生成物类 GlyphSpawnActor；Context: 生成物上下文 | 根据 FGlyphSpawnActorContext 配置生成 AC_GlyphSpawnActor 实例。当前实现支持两种生成模式：None（单发）、Orbit（环绕多枚）。枚举中保留 RapidFire/Scatter，但尚未实现；如需启用再补。生成物自动持有当前刻印的弱引用，用于 OnTravel 事件广播和变位交互。 |

#### 3.4.3. 碰撞检测函数

| 函数 | 参数 | 说明 |
|---|---|---|
| SphereCollisionOverlapCheck | AvatarActor: 发起者；HitBoxRadius: 球体半径；HitBoxForwardOffset: 前方偏移；HitBoxElevationOffset: 上方偏移；bDrawDebug: 是否绘制调试 | 以 AvatarActor 位置为基准，添加前方偏移与上方偏移后执行球体通道重叠检测（ECC_Pawn），返回检测到的 AC_BaseCharacter 数组。 |
| BoxCollisionOverlapCheck | AvatarActor: 发起者；BoxSize: 盒体尺寸；HitBoxForwardOffset: 前方偏移；HitBoxElevationOffset: 上方偏移；bDrawDebug: 是否绘制调试 | 以 AvatarActor 位置为基准，添加前方偏移与上方偏移后执行盒体通道重叠检测（ECC_Pawn），返回检测到的 AC_BaseCharacter 数组。 |
| LineCollisionHitCheck | AvatarActor: 发起者；Length: 射线长度；HitBoxForwardOffset: 前方偏移；HitBoxElevationOffset: 上方偏移；bDrawDebug: 是否绘制调试 | 以 AvatarActor 位置为基准，添加前方偏移与上方偏移后沿角色朝向执行射线通道命中检测（ECC_Pawn），返回检测到的 AC_BaseCharacter 数组。 |

#### 3.4.4. 伤害与效果应用函数

| 函数 | 参数 | 说明 |
|---|---|---|
| ApplyDamageToTarget | Target: 目标角色；Damage: 原始伤害；Attribute: 伤害元素；DamageEffect: 伤害 GE；OptionalParticleSystem: 可选特效 | 即时伤害的统一入口。校验目标后构建 FGameplayEffectContextHandle，通过 AddSourceObject 将当前刻印附加到上下文；用 SetByCaller 写入 Damage，把元素枚举转换为 `CTags.Datas.Elements.*` 动态标签，应用伤害 GE；随后向目标发送 `CTags.Events.Hit` 并广播本位 OnHit。返回原始伤害值，不做修饰计算。 |
| ApplyEffectToTarget | Target: 目标角色；Effect: GE 类；Duration: 持续时间；Level: 效果等级 | 通用效果应用入口。通过 SetByCaller 传递持续时间，以 Level 调用 MakeOutgoingSpec，再应用到目标 ASC。DOT/Buff 的元素类型由 GE 蓝图静态配置。 |

### 3.5. 刻印制作流程

1. 创建继承自 UGlyphBase 的蓝图类。
2. 配置基础信息（GlyphID、名称、描述、图标、最大等级）。
3. 分别实现三个功能槽的本位入口：`AttackBase` / `SkillBase` / `MoveBase`，在关键节点调用 `BaseEvent` 广播生命周期事件。
4. 实现变位逻辑：`PreProcessContext` 修改运行配置，`PreActivate` 做激活前准备，`BaseEventReceived` 处理事件增量；按 `GlyphType` 分支处理功能槽差异。
5. 在 `DT_Affix` 中配置词条池与层级区间；刻印实例用 `FGlyphAffix` 保存抽取结果，词条数值随层数成长。
6. 配置 `TArray<FGlyphChargeContext>` 蓄力段与对应蒙太奇（可选）。
7. 配置生成物上下文、碰撞参数、BaseTags 等数据驱动字段。
8. 在测试关卡中验证三槽本位差异、变位预处理与事件增量效果。

---

## 4. AI 系统设计

> 本章适用于 Boss 波刻印怪。小怪为固定行为，不套用本 AI 系统。

### 4.1. 黑板设计

当前已实现的黑板键：

- `TargetActor`：当前目标（玩家）
- `AttackGlyph` / `SkillGlyph` / `MoveGlyph`：各槽位刻印名
- `AttackInCD` / `SkillInCD` / `MoveInCD`：各功能槽是否在冷却

后续补齐：

- `DistanceToTarget`：与目标距离
- `HealthPercent`：自身生命百分比
- `bIsUnderThreat`：是否受到威胁
- 槽位可用状态
- 当前行为标签（Melee / Ranged / AOE / Dash / Escape / Heal / Summon）

### 4.2. 行为树结构

行为树采用固定骨架 + 时机驱动标签查询：

- 根节点选择器按优先级查询：位移决策 → 技能决策 → 普攻决策。
- 位移决策节点：刚被赋予或受到威胁时触发，读取位移槽本位标签，执行 Dash 或 Escape；无标签/空槽不主动使用位移。
- 技能决策节点：技能槽冷却完毕时触发，读取技能槽本位标签，按 Melee/Ranged/AOE/Heal/Summon 分支执行对应移动与释放逻辑；无标签默认近战范围释放。
- 普攻决策节点：技能槽冷却中时触发，读取普攻槽本位标签，按 Melee/Ranged 分支循环普攻；无标签/空槽默认近战挥击。

### 4.3. BTTask 设计

#### 4.3.1. 统一执行模式

所有 BTTask 不直接执行业务逻辑。Task 只负责决策与目标选择，执行时通过 AbilitySystemComponent 按 `CTags.Abilities.Attack/Skill/Move` 激活对应功能槽 Ability；Ability 再把执行权交给 InventoryComponent 与刻印。空槽回退由 ActivateSlotGlyph 内部处理。

#### 4.3.2. 位移类任务

- `BTT_MoveToByMoveTag`：读取位移槽本位标签；`Dash` 向玩家方向突进 400~600，`Escape` 远离玩家 500~800；无标签/空槽不使用位移。
- `BTT_TowardTarget`：朝目标移动的通用任务，供无位移语义时回退。

#### 4.3.3. 技能类任务

- `BTT_MoveToBySkillTag`：读取技能槽本位标签，按 `Ranged` / `Melee` / `AOE` / `Heal` 选择目标距离或目标位置。
- `BTT_Skill`：按 `CTags.Abilities.Skill` 激活技能槽 Ability。
- `Heal` 需要同时检查 `HealthPercent < 50%`；`AOE` 移动到玩家当前位置；`Summon` 在玩家周围选点。以上分支复用通用移动/释放任务 + 装饰器，不新建专用任务。
- 无标签/空槽：默认近战范围释放。

#### 4.3.4. 普攻类任务

- `BTT_MoveToByAttackTag`：读取普攻槽本位标签，按 `Ranged` / `Melee` 选择目标距离。
- `BTT_Attack`：按 `CTags.Abilities.Attack` 激活普攻槽 Ability。
- 无标签/空槽：默认近战挥击回退。

### 4.4. BTService 设计

- `BTS_AbilityCooldown`（已实现）：读取 `Tags.Cooldown.Attack/Skill/Move`，更新各槽位冷却布尔。
- `BTS_UpdateTargetInfo`（计划）：更新目标角色、距离、生命百分比。
- `BTS_UpdateThreat`（计划）：检测是否受到威胁，供位移决策使用。

### 4.5. 装饰器设计

- `BTD_CanAttack` / `BTD_CanSkill` / `BTD_CanMove`（已实现）：检查对应槽位冷却与目标有效性。
- `BTD_CheckTag`（计划）：检查当前槽位刻印本位标签是否匹配。
- `BTD_CheckDistance`（计划）：检查与目标距离是否在阈值内。
- `BTD_CheckHealthPercent`（计划）：检查生命百分比（用于 Heal 决策）。

---

## 5. 环境与表现系统

### 5.1. 场景结构

场景为单一后室风格封闭空间，层数切换以坠落转场实现。

### 5.2. 环境异化系统

环境异化通过材质参数与后处理驱动，从墙纸剥落、灯光频闪，到表面渗液、边界消融、空间扭曲逐层递进。

每五层触发随机环境异象，包括光源衰减、重力偏移、空间错位等，持续整层。

### 5.3. 视觉反馈规范

- 刻印怪外观：发光颜色体现信息素与抗性（红→紫渐变警示），体型体现刻印数与大类型，具体刻印在战斗中揭示。
- 信息素累积与吸收：小怪波中信息素累积需有视觉提示；Boss 波吸收动画作为转场核心表现。
- 词条获取：爆装时展示词条类型与数值，越深词条数值越高。
- 顿悟触发：概率触发与 Boss 必触发需有明确视觉反馈。
- 抗性警示：刻印怪抗性以渐变色纹路警示。

---

## 6. 技术架构

### 6.1. 项目结构

```
Source/Glyph/
  Ability/          GAS 属性集、功能槽 Ability 壳、GameplayTags
  Character/        角色基类
  Enemy/            敌人基类、小怪/刻印怪类、敌人血条
  Glyph/            UGlyphBase、UGlyphInventoryComponent、生成物、持续事件组件
  Map/              GameMode、GameState、信息素表现 Actor
  Player/           玩家角色、PlayerController、PlayerState
  UI/               UI 组件、库存 Widget C++ 接口
  Affix/            词条数据结构与抽取（计划）
  Pheromone/        信息素账户、权重与抗性计算（计划）
  Camera/           双视角相机组件与输入切换（计划）
  Save/             残响图鉴、初始池、槽位预存序列化（计划）

Content/Game/
  AbilitySystem/    Ability / Cue / Effect
  Anim/Notify/      动画通知
  Data/             DT_Glyph、DT_Affix、敌人/波次/环境配置
  Enemy/AI/         行为树、黑板、BTTask、BTService、Decorator
  Glyph/            刻印蓝图与生成物蓝图
  Input/            Enhanced Input
  Map/              地图与 GameMode 蓝图
  Player/           玩家蓝图
  UI/               UMG 布局与动效
```

### 6.2. C++/蓝图分工

| 模块 | C++ | 蓝图 |
|---|---|---|
| 刻印基类与库存 | 核心逻辑、容量/重复/瞬切校验 | 刻印具体实现、配置默认值 |
| 词条系统 | `FGlyphAffix`、抽取与运行配置装配 | 词条池 `DT_Affix` 配置 |
| 信息素系统 | `AC_GameState` 层内账户、吸收、权重、抗性计算 | 信息素表现、UI |
| 敌人 | 敌人基类、小怪/刻印怪类拆分、生成与死亡 | 敌人外观、参数、行为树资产 |
| 融合系统 | `UC_FusedGlyph` 包装、主源/副源路由、冲突规则 | 融合界面、融合表现 |
| 双视角 | 相机框架与输入切换 | 相机参数配置 |
| AI | 通用 BTTask/BTService/Decorator 基类（如需） | 行为树资产配置 |
| UI | 数据绑定接口、校验 | 布局与动效 |
| 存档 | 序列化 | — |

### 6.3. 核心类设计

#### 6.3.1. UGlyphInventoryComponent（库容管理）

职责：持有 8 格库容、三槽两位格配置、刻印等级与词条、残响转化触发、槽位配置预存与瞬切、重复获取二选一、容量/唯一性校验、空槽回退。

关键接口：
- `AddGlyph`：创建实例并加入库容；目标实现 8 格上限与同一刻印类唯一性校验。
- `RemoveGlyph`：移除并清空对应槽位；目标实现残响转化钩子（仅玩家主动舍弃/替换时触发）。
- `SetGlyphType`：移动/交换刻印到指定槽位；当前已实现。
- `ActivateSlotGlyph(SlotType, AbilityRef)`：三槽统一执行入口；当前已实现。
- `EndRunningCharge` / `SettleCharge`：蓄力中断与结算；当前已实现。
- `GetSlotContent` / `NameToGlyph`：槽位查询；当前已实现。
- `ReplaceGlyph` / `OnDuplicateGlyph`：替换与重复二选一；计划。
- `SavePreset` / `SwitchPreset`：预存实例引用、切换校验/回滚、5 秒冷却；计划。
- `ConvertToResonance`：舍弃/替换/被放弃刻印转残响；计划。
- 空槽回退：普攻默认近战挥击、技能为空、位移默认翻滚；计划。

#### 6.3.2. UGlyphBase（刻印基类）

职责：刻印基础信息、三槽本位入口、生命周期事件广播/接收、变位预处理、工具函数、词条字段与词条数值读取接口。

关键字段：`GlyphID`、`GlyphName`、`GlyphAttribute`、`GlyphType`、`MaxLevel` / `CurrentLevel`、`TArray<FGlyphAffix>`、三套 `FGlyphConfigurationContext`、`RunningConfiguration`、`OwningAbility`。

关键接口：
- `AttackBase` / `SkillBase` / `MoveBase`（BlueprintImplementableEvent，三槽本位入口）
- `ActivateGlyph`（按 GlyphType 分发）、`ApplyCooldown`
- `BaseEvent(EventType, Context)`（本位广播）
- `BaseEventReceived(EventType, Context)`（变位蓝图接收）
- `PreProcessContext(SlotType, Context)`（变位修改运行配置）
- `PreActivate()`（激活前准备）
- `CreateWaitGameplayEventTask` / `CreatePlayMontageAndWaitTask`
- `SpawnActor` / `SphereCollisionOverlapCheck` / `BoxCollisionOverlapCheck` / `LineCollisionHitCheck`
- `ApplyDamageToTarget` / `ApplyEffectToTarget` / `TryEndAbility`
- `GetAffixValue(AffixType)`（计划）

#### 6.3.3. UTravelEventComponent（持续事件转发组件）

职责：挂载于需要持续广播 OnTravel 事件的对象上（角色或刻印生成物），以时间间隔或移动距离间隔触发事件广播。

#### 6.3.4. UAttributeSet（属性集）

职责：生命值、最大生命、`IncomingDamage` 元属性、四种元素抗性；通过 `PostGameplayEffectExecute` 统一处理伤害、抗性、格挡与减伤，并在生命归零时触发死亡。

- 灵感值不存在；顿悟由概率/必触发控制。
- 词条数值不进入 AttributeSet，由刻印词条字段在装配 RunningConfiguration 时读取。
- 抗性默认 0；刻印怪生成时按吸收信息素占比写入四种抗性（0~0.6）。

#### 6.3.5. 敌人类拆分

```
AC_BaseCharacter
└── AC_EnemyBase（共享：ASC/AttributeSet、死亡、信息素记录）
    ├── AC_SmallMobCharacter（固定小怪：固定能力/外观，不持有 Inventory）
    └── AC_GlyphMonsterCharacter（刻印怪：Inventory、三槽、体型/变位、AI）
```

- 现有 `AC_EnemyCharacter` 的“三槽 + GiveGlyph + AI”逻辑迁入 `AC_GlyphMonsterCharacter`。
- `AC_SmallMobCharacter` 使用固定能力/蒙太奇与固定外观，死亡时仍按元素记录信息素。
- 两者共享敌人基类的生命、死亡、信息素与掉落接口，避免 AI/三槽逻辑耦合。

#### 6.3.6. 融合刻印 UC_FusedGlyph

融合刻印是 `UC_GlyphBase` 的包装对象，持有主源与副源的引用：

- `MainGlyph`：提供三套本位配置、等级与词条；
- `SecondaryGlyph`：仅提供变位逻辑；
- 本位入口（AttackBase / SkillBase / MoveBase）转交主源；
- `BaseEventReceived` 转交副源的变位逻辑；
- `PreProcessContext` 以主源运行配置为基础，调用副源变位进行修改；
- 融合刻印占 1 个库容格，不可再融合或升级；
- 主源/副源在融合时确定，源刻印从库容移除并由融合对象持有，融合对象销毁时一并释放。

#### 6.3.7. 信息素账户 FPheromoneData / AC_GameState

- `AC_GameState` 持有本层 `FPheromoneData` 账户。
- 小怪死亡时按击杀刻印的 `GlyphAttribute` 累加；`None` 不计入。
- Boss 波开始时读取并清空本层账户，作为刻印怪生成材料。
- 抽取权重：与信息素元素相同的刻印获得 3 倍权重。
- 抗性：按元素占比 × 60% 写入刻印怪 AttributeSet。
- `AC_Pheromone` 仅负责死亡点特效/视觉提示，不保存层内数据。

### 6.4. 数据驱动配置

#### 6.4.1. 刻印生成物上下文 FGlyphSpawnActorContext

字段：`Damage`、`DamageAttribute`、`Size`、`ForwardOffset`、`ElevationOffset`、`bOverrideRotation` / `Rotation`、`FireSpeed`、`bCanPenetrate`、`bOverrideOrigin` / `Origin`、`SpawnType`、`Number`、`ContinuousTime`、`ScatteringAngle`、`OrbitDistance`、`OrbitAngleSpeed`。

- `SpawnType` 枚举包含 None / RapidFire / Scatter / Orbit；当前仅实现 None / Orbit。
- 当前没有寿命字段；如后续需要生成物存活时间，再增加并写入 `LifeSpan`。

#### 6.4.2. 刻印蓄力上下文 FGlyphChargeContext

每段字段：`ChargeTime`、`ChargeMontage`、`DamageRate`、`CollisionSizeRate`、`SpawnActorDamageRate`、`SpawnActorSizeRate`、`SpawnActorSpeedRate`、`EffectTimeRate`、`MoveFactorRate`。

`FGlyphConfigurationContext.ChargeContexts` 为 `TArray<FGlyphChargeContext>`。运行期的 `ChargeIndex`、`ChargeStartTime`、`IndexChangeTime` 由 InventoryComponent 保存，不进入配置结构。

#### 6.4.3. 刻印配置上下文 FGlyphConfigurationContext

字段：`Cooldown`、`Damage`、`DamageAttribute`、`CollisionSize`、`HitBoxForwardOffset`、`HitBoxElevationOffset`、`MontageRate`、`SpawnActorContext`、`ChargeContexts`、`EffectTime`、`MoveFactor`、`BaseTags`。

`RunningConfiguration` 是同结构的运行时副本：InventoryComponent 先复制本位配置，再经过变位 `PreProcessContext` 与蓄力结算。等级与词条的并入顺序见 9.4。

#### 6.4.4. 刻印事件上下文 FGlyphEventContext

字段：`BaseGlyph`、`EventLocation`、`EventToward`、`HitActor`。

事件上下文为只读通知数据；不包含当前伤害、浮点参数映射或对象参数映射，变位不修改事件上下文。数值修饰在 `PreProcessContext` 阶段完成。

#### 6.4.5. 词条 FGlyphAffix

- `AffixType`：词条类型（伤害加成、冷却缩减、范围增加等，可用 GameplayTag 或枚举）。
- `Value`：数值。
- `TierMin` / `TierMax`：层级区间。
- 词条池配置在 `DT_Affix`；刻印实例保存抽取结果。
- Demo 每枚刻印 1 条属性词条。

#### 6.4.6. 信息素 FPheromoneData

- `TMap<EGlyphAttribute, float> Amounts`：本层各元素累积量。
- `TotalAmount`：本层总量，用于计算占比。
- `Add(Element, Amount)`、`Clear()`、`Absorb()`（返回快照并清空）。
- 由 `AC_GameState` 持有；`AC_Pheromone` 仅做视觉。

#### 6.4.7. 槽位预存 FSlotPreset

- 6 个放置位的刻印实例引用/ID。
- 切换时校验引用是否仍在库容、未被融合或舍弃；失败拒绝切换。
- 瞬切冷却 5 秒（可调）；初版 1 套。

#### 6.4.8. 数据表与数据资产分工

| 数据 | 载体 | 说明 |
|---|---|---|
| 刻印行为配置 | 蓝图默认值 + `FGlyphConfigurationContext` | 核心技能逻辑与参数，不放入 DataTable |
| 刻印展示信息 | `DT_Glyph` | 名称、描述、图标等；当前已有 |
| 词条池 | `DT_Affix` | 词条类型、数值区间、层级区间 |
| 敌人/波次配置 | `DT_Enemy` / `DT_Wave` | 体型权重、槽位概率、波次组成 |
| 环境异象 | `DT_Environment` | 异象类型、触发条件、参数 |
| 残响初始池 | `DataAsset_Resonance` | 解锁条件与初始池选项 |
| 残响/预存存档 | SaveGame | 序列化持久化 |

> 原则：核心行为配置跟随刻印蓝图/结构体；DataTable/DataAsset 只承担静态内容、展示与可批量编辑的数据。

### 6.5. UI 架构

- 开库半透明侧边栏：8 格库容 + 三槽两位图示；可见战场、可移动、不可攻击；展示等级、词条、融合状态。
- 刻印详情：名称、元素、描述、图标、等级、词条、当前位置。
- 重复二选一与舍弃：保留旧刻印/换新刻印；满库舍弃；被放弃刻印提示转残响。
- 顿悟界面：融合/升级/跳过；融合时选择两枚同级刻印与主源。
- 残响图鉴：展示已解锁残响与初始池选项。
- 瞬切配置与提示：当前预存配置、切换冷却、失效引用提示。
- 战斗 HUD：信息素按元素分色的累积条、刻印怪行为标签、伤害数字、技能冷却。
- 双视角 UI：小怪波俯视角 UI 与 Boss 波越肩 UI 切换。

---

## 7. 开发计划

### 7.1. 开发里程碑

- **M0 基线修复**：修复已知 bug；实现库容 8 格与唯一性；确定事件/配置预处理口径；更新文档与工具。
- **M1 单层纵向切片**：用现有 8 枚刻印跑通“小怪波 → Boss 波 → 顿悟 → 坠落”；实现空槽回退、敌人拆分最小版、基础信息素、双视角与最小 HUD。
- **M2 组合与成长**：完成刻印数据模型、重复二选一、等级/词条、融合、残响、存档、槽位预存/瞬切与相关界面。
- **M3 内容与敌人扩展**：补齐分层 AI、12~16 枚刻印、5~8 层、环境异象、体型/变位规则、战斗 HUD 与灰盒表现。
- **M4 打磨与交付**：引导与平衡、独立打包、性能优化、配置校验、测试与验收记录。

具体缺陷、改造项与待开发条目见《项目整理.md》的 M-/T- 清单；GDD 只描述目标与验收口径。

### 7.2. 刻印清单与实现阶段

- **已实现 8 枚**：火痕、燃屑、露珠、浸润、轻风、间隙、沉砾、岩片；均已具备三槽本位与一套变位逻辑。
- **目标 12~16 枚**：覆盖 Melee / Ranged / AOE / Dash / Escape / Heal / Summon 行为标签与 Fire / Water / Wind / Soil 元素标签；每枚实现三槽本位逻辑与一套通用变位逻辑；词条池仅属性类。
- **新增内容优先补**：AOE / Heal / Summon 语义、元素覆盖、融合边界用例（同字段冲突、库容溢出）。
- **每枚刻印**：Demo 阶段 1 条属性词条；等级上限 3。

### 7.3. Demo 范围

- 5~8 层，每层“小怪波 → Boss 波”双段。
- 每五层一次环境异象，Demo 至少一次。
- 一个 Boss 波刻印怪；初版单只，多只稍弱作为扩展。
- 12~16 枚可组合刻印，词条池仅属性类。
- 轮回 1~2 内置引导。
- 小怪俯视角与 Boss 越肩双视角。

### 7.4. 当前实现状态

> 状态标记：✅ 可用　🟡 部分实现　🔴 未实现

| 模块 | 当前状态 | 状态 |
|---|---|---|
| GAS、属性集、伤害/抗性 | 玩家/敌人 ASC、元属性伤害、四抗性、DOT/Buff GE | ✅ |
| 刻印基类与三槽两位格 | UObject 基类、三套配置、本位入口、变位预处理/事件 | ✅ |
| 8 枚 MVP 刻印 | 三槽本位 + 变位逻辑 + 蒙太奇/生成物 | ✅ |
| 库容/重复/等级/词条 | `AddGlyph` 未做 8 格、唯一性与重复处理；无等级/词条字段 | 🔴 |
| 信息素 | `AC_Pheromone` 死亡点 + GameState 查询，未按元素累积/吸收 | 🟡 |
| 敌人 | 所有敌人共用三槽；未拆分小怪/刻印怪；体型/变位规则未完整 | 🟡 |
| AI | 通用标签移动/释放任务 + 冷却装饰器/服务；缺距离/生命/威胁 | 🟡 |
| 波次/双视角/环境 | 仅 BP 波次雏形；第三人称单相机；无环境异象 | 🔴 |
| 顿悟/融合/残响/瞬切 | 未实现 | 🔴 |
| UI | 库存、槽位、详情、敌人血条；缺图鉴/融合/词条/信息素/瞬切 | 🟡 |

---

## 8. 验收清单

### 8.1. 功能验收

按里程碑验收，M0 为基线，M1 起为可玩内容：

- **M1**：双段波次切换正常，转场可跳过；固定小怪可击杀；Boss 刻印怪可由信息素生成；信息素累积/吸收归零；双视角与输入切换正确；基础顿悟（升级/跳过）可用。
- **M2**：刻印三槽本位差异正确，变位预处理/事件增量生效；库容 8 格与唯一性正确；重复获取二选一、被放弃刻印转残响；等级上限 3、词条随层数成长；融合（主源本位 + 副源变位）可用；残响图鉴/存档/初始池可用；槽位瞬切带 5 秒冷却且切换校验正确。
- **M3**：12~16 枚刻印覆盖七类行为标签与四元素；刻印怪配置向信息素偏好趋同，抗性按占比生效；体型/槽位规则正确；AI 覆盖 Melee/Ranged/AOE/Dash/Escape/Heal/Summon；5~8 层与至少一次环境异象；Boss 标签可读。
- **M4**：引导、平衡、打包、性能与配置校验通过；可从头玩到 Demo 结束。

### 8.2. 技术验收点

- GAS 功能槽 Ability 仅作入口，业务逻辑在刻印 UObject 中。
- 变位 `PreProcessContext` 修改运行配置的链路无泄漏；事件上下文保持只读；事件广播与 Ability 生命周期检测正确。
- AbilityTask C++ 中转方案稳定；蓄力独立调度，Ability 结束/取消时蓄力中断且不误激活。
- 库容 8 格/唯一性/重复二选一/舍弃替换均通过事务式校验，不产生半更新状态。
- 信息素层内账户、Boss 吸收归零、同元素 3 倍权重、抗性占比计算正确且性能达标。
- 融合包装对象的主源/副源路由、等级/词条继承与不可再融合规则正确。
- 通用标签 AI 任务 + 装饰器/服务行为符合 GDD；变位标签不参与决策。
- `DT_Glyph` / `DT_Affix` / `DT_Enemy` / `DT_Wave` / `DT_Environment` 配置与读取正确。
- 双视角相机与输入切换无冲突；残响/预存存档序列化完整。

---

## 9. 附录

### 9.1. 术语表

| 术语 | 含义 |
|---|---|
| 刻印 | 构筑基础单元，含本位与变位逻辑，由随机语法与随机词条组成 |
| 本位 | 技能核心行为骨架，界定"做什么" |
| 变位 | 技能修饰行为模块：通过 PreProcessContext 修改运行配置，或接收本位事件执行增量逻辑 |
| 功能槽 | 普攻/技能/位移三个槽位 |
| 位格 | 每个功能槽的两个位置，槽位一放本位，槽位二放变位 |
| 库容 | 刻印持有上限，固定 8 格 |
| 顿悟 | 触发即用的强化机会，可融合/升级/跳过 |
| 融合主源/副源 | 主源提供本位逻辑、等级与词条；副源只提供变位逻辑 |
| 残响 | 被舍弃/替换/放弃的刻印转化物，录入图鉴解锁初始池 |
| 信息素 | 小怪波按元素累积的层内账户，Boss 波吸收用于喂养刻印怪 |
| 刻印怪 | Boss 波敌人，复用玩家三槽架构，配置向信息素趋同 |
| 词条 | 刻印的数值附魔，与等级分离，随层数成长 |

### 9.2. 设计决策记录

#### 9.2.1. 刻印设计
每枚刻印为一个独立的蓝图类，本位和变位逻辑写在同一个文件中。不复用整枚刻印的逻辑，只复用工具函数。本位执行行为并在关键节点广播事件；变位在激活前通过 `PreProcessContext` 修改运行配置，或通过 `BaseEventReceived` 响应事件做增量逻辑。两者完全解耦，本位不持有变位引用。三个功能槽各有一套本位逻辑，变位只做一套通用于所有槽位。基类提供通用工具函数供刻印调用（碰撞检测、生成物生成、AbilityTask 创建等），覆盖绝大多数需求。

#### 9.2.2. 刻印基类使用 UObject 而非 AActor
刻印在游戏中无物理实体（无Transform、无碰撞、无模型），仅作为逻辑和数据的容器，因此UGlyphBase继承自UObject而非AActor。此举的核心价值在于生命周期管理的简化，UObject可由UGlyphInventoryComponent直接持有，随组件销毁而自动回收，无需经由UWorld调度；同时避免了AActor所附带的Transform、Component、Tick、Replication等与刻印语义无关的系统性开销，使刻印在架构层面保持纯粹的"逻辑物件"身份，符合其作为技能构筑单元的设计初衷。

#### 9.2.3. 工具函数以基类成员函数提供而非独立组件
工具函数原设计方案为独立的 UObject 组件，由刻印通过 NewObject 创建并持有。实际开发中发现，所有刻印几乎都需要调用这些函数（碰撞检测、生成物生成、AbilityTask 创建），若拆分为组件，则每枚刻印需额外持有组件引用并管理其生命周期，增加内存开销与使用复杂度。决策将工具函数直接作为 UC_GlyphBase 的成员函数提供。刻印基类 UObject 本身已承载逻辑与数据，工具函数本质上是刻印行为的扩展，归属于基类是自然的职责划分。成员函数调用无额外寻址开销，调用方式简洁（直接调用而非 Component->Function()），且无需额外的组件创建与销毁逻辑。

#### 9.2.4. 功能槽 Ability 作为执行壳
刻印继承UObject而非GameplayAbility，游戏仅提供Attack、Skill、Move三个功能槽Ability作为GAS激活入口。刻印本质是"物品"（可拾取、拖拽、融合、升级、存档），UObject比Ability更适合作为物品载体，且50+刻印若每个都创建一个Ability类将产生大量冗余资产。功能槽Ability为薄壳，激活时调用InventoryComponent->ActivateSlotGlyph(SlotType, this)将自身引用传递给本位刻印，本位刻印通过AbilityRef调用GAS原生函数，能力与Ability方案完全一致。刻印不持有AbilityRef，仅作为函数参数传递，通过IsValid()检测生命周期，无需手动绑定解绑。本位刻印广播事件时不携带AbilityRef，变位通过InventoryComponent获取本位刻印引用进行交互，保持事件系统的纯净性。变位通过PreProcessContext修改运行配置，或在BaseEventReceived中响应事件，不修改事件上下文。

#### 9.2.5. AbilityTask C++ 中转方案
刻印基类为UObject子类，而AbilityTask的蓝图节点被BlueprintInternalUseOnly元数据限制，只能在UGameplayAbility内部使用，UObject无法直接调用。因此需要在UGlyphBase基类中提供C++级别的中转函数（CreatePlayMontageAndWaitTask、CreateWaitGameplayEventTask等），内部调用GAS原生工厂函数并显式调用ReadyForActivation()激活Task，将Task指针返回给刻印蓝图使用。此举以极小封装成本（每个Task约5行代码）解决了UObject与GAS异步任务之间的调用鸿沟，后续新增AbilityTask只需按相同模式扩展，刻印蓝图可像原生Ability一样绑定Task回调，不破坏刻印的UObject属性（存档、库容、融合能力）。

#### 9.2.6. 变位影响本位的配置预处理与事件机制
变位对本位的影响采用“激活前配置修改 + 事件挂载增量逻辑”两层机制，管理层覆盖暂不实现。

1. **激活前配置修改（主要方式）**：InventoryComponent 先复制本位刻印的 `FGlyphConfigurationContext` 为 `RunningConfiguration`，再调用变位的 `PreProcessContext(SlotType, Context)` 修改该运行配置，最后才执行本位。变位可修改伤害、碰撞尺寸、生成物数量/速度、效果持续时间、位移系数、蓄力段参数等。本位只读取运行配置，不知道变位存在。
2. **事件挂载增量逻辑（次要方式）**：变位通过 `BaseEventReceived` 监听本位广播的 `OnCast` / `OnHit` / `OnFinish` 等事件，执行额外逻辑（命中后挂 DOT、触发爆炸、播放特效等）。事件上下文只读，变位不修改事件数据。
3. **管理层覆盖（暂不实现）**：极端情况下由 InventoryComponent 判断是否由变位接管执行；当前阶段不实现，待有真实需求再引入。

设计原则：本位不持有变位引用且不预留逻辑分支；变位通过修改运行配置或响应事件来影响本位，不直接调用本位内部函数；管理层负责路由决策但不参与业务逻辑。事件上下文只承担通知职责，数值修改统一在激活前完成，避免同一数值存在多个修改入口。

#### 9.2.7. 蓄力系统设计
蓄力系统的设计遵循与刻印系统一致的"数据驱动+事件驱动"原则，核心决策在于不依赖 GAS 的 AbilityTask，而是将蓄力状态交由 UGlyphInventoryComponent 统一调度。理由是刻印本质是 UObject 物品而非 Ability，若为蓄力单独引入 AbilityTask 机制，则每枚刻印的蓄力逻辑将散落在各自 Ability 中，既破坏刻印作为独立单元的封装性，也与"功能槽 Ability 仅作为执行壳"的架构相悖。统一调度确保任意刻印无需额外实现即可获得蓄力能力，符合"刻印作为独立单元"的设计语义。Ability 结束/取消时由 InventoryComponent 中断蓄力并停止蒙太奇；蓄力完成时先检查 `OwningAbility` 有效，再执行激活，避免在 Ability 失效后创建/绑定 AbilityTask。

#### 9.2.8. 元属性伤害处理架构
采用元属性（IncomingDamage）+ PostGameplayEffectExecute 处理伤害，而非使用 GE Modifier 或 ExecutionCalculation。因为伤害来源多样（即时技能、DOT、反射、环境陷阱），若每个来源各自处理抗性和减伤，逻辑将分散在多处，新增伤害类型时必须重复实现相同的修饰公式，维护成本和出错风险随项目扩张线性增长。元属性方案通过 IncomingDamage 作为统一数据管道，强制所有伤害来源（无论 ApplyDamageToTarget 还是 DOT 周期 GE）修改同一属性，在 PostGameplayEffectExecute 中汇集所有伤害进行统一修饰。这种多入口单出口模式将抗性、减伤、格挡等公式集中在一处维护，新增伤害来源无需复制任何逻辑。相较于 GE Modifier（逻辑分散至多个 GE）、ExecutionCalculation（逻辑隔离在独立 C++ 类，无法直接复用 AttributeSet 的属性和函数），元属性方案与 AttributeSet 处于同一文件，可直接访问所有抗性属性和 Tag，逻辑内聚且便于调试。

#### 9.2.9. 小怪不套用三槽架构
小怪为固定设计怪物，行为与外貌固定，降低小怪波复杂度，突出割草爽感；三槽架构保留给 Boss 波刻印怪，形成双段节奏差异。

实现上拆分为 `AC_SmallMobCharacter` 与 `AC_GlyphMonsterCharacter`，两者共享敌人基类：小怪不持有 `UC_GlyphInventoryComponent`，刻印怪持有完整三槽与 `GiveGlyph`。这样 AI、三槽规则和外观逻辑不会混在同一个类中。

#### 9.2.10. 顿悟不可攒、触发即用
顿悟改为概率/必触发，避免玩家囤积资源导致节奏拖沓，强制即时决策，强化 Rogue-like 的随机与取舍。初版参数：小怪击杀约 5% 概率触发（可调），Boss 必触发；触发后只能在融合/升级/跳过中选择，不保留到下一次。

#### 9.2.11. 本版技术方案决策（2026-11-28）

| 决策 | 采用方案 |
|---|---|
| GDD 定位 | GDD 承载完整技术方案；未确认细节集中在 9.4 |
| 变位/事件 | 变位不修改事件上下文；通过 `PreProcessContext` 修改运行配置，`BaseEventReceived` 仅做增量逻辑 |
| 敌人架构 | C++ 拆分小怪类与刻印怪类，共享敌人基类 |
| 数据载体 | 核心行为用结构体 + 蓝图默认值/运行上下文；静态内容/展示/词条池用 DataTable/DataAsset |
| 信息素 | `AC_GameState` 持有层内账户；`AC_Pheromone` 仅做表现 |
| 融合 | `UC_FusedGlyph` 包装对象持有主源/副源；主源本位 + 副源变位 |
| AI 任务 | 保留并扩展通用标签任务 + Decorator/Service；不建专用 CastSkill_* 任务 |
| 瞬切 | 预存刻印实例引用，切换校验/失败回滚，5 秒冷却 |
| 蓄力 | InventoryComponent 独立调度；Ability 结束/取消时中断；完成时检查 Ability 有效性 |
| 术语 | 元素保留 Wind/Soil；行为标签采用 Ranged/AOE/Heal/Summon，迁移 Range/Effect |

### 9.3. 问题修复记录

#### 9.3.1. AbilityTask 激活与清理问题
在开发过程中，C++ 中创建的 AbilityTask 在技能结束后没有被正确清理，导致内存残留和意外回调触发。该问题主要由 AbilityTask 的激活方式引起，具体描述如下：
UGameplayAbility 通过 ActiveTasks 数组管理所有关联的 AbilityTask。当技能结束时，EndAbility 会遍历该数组并清理所有任务。AbilityTask 的正确激活流程分为两步：先将自身注册到 ActiveTasks 数组，再执行内部逻辑。在 C++ 中，ReadyForActivation() 函数封装了这一完整流程，而 Activate() 仅执行内部逻辑，不负责注册。开发初期，在 CreateWaitGameplayEventTask 和 CreatePlayMontageAndWaitTask 等工厂函数中直接调用了 Task->Activate()，而非 ReadyForActivation()。这导致创建的 AbilityTask 虽然内部逻辑正常运行，但未被添加到 ActiveTasks 数组中。当技能结束时，EndAbility 遍历 ActiveTasks 数组找不到该任务，跳过了清理步骤，造成任务残留。正确做法是统一使用 ReadyForActivation() 激活所有 AbilityTask。修正后的写法如下：创建 Task 后先判断有效性，再调用 ReadyForActivation()，将注册和激活合并为一步完成。修正后，EndAbility 能够正常清理所有任务，不再需要额外的 ManagedTasks 数组或手动清理逻辑。在开发的其他模块中，凡是涉及 C++ 创建 AbilityTask 的地方，统一遵循此规范。

#### 9.3.2. 第一次播放蓄力蒙太奇无动画
在实现蓄力系统时，首次触发蓄力时角色无任何动画表现，蒙太奇未被播放，第二次及之后触发则正常播放。该问题仅在打包后的独立游戏中出现，开发环境中不易复现。
问题根源在于 AbilityTask_PlayMontageAndWait 的内部行为。该任务通过 Montage_Play 播放蒙太奇，但任务的 Activate() 执行并非同步完成，中间存在帧间隔。首次调用时，蒙太奇资源可能尚未完全流送至内存，AbilityTask 激活过程中的异步注册机制与此冲突，导致 Montage_Play 被跳过或返回 0.0f。由于 AbilityTask 不会在失败后自动重试，第一次播放就此失败，动画不显示。后续调用时资源已在内存中，播放正常。解决方案是将蓄力蒙太奇的播放与 AbilityTask 解耦，不再通过 CreatePlayMontageAndWaitTask 播放蓄力动画。蓄力过程由 UGlyphInventoryComponent 独立管理，通过 AnimInstance->Montage_Play() 直接在动画实例上同步播放蒙太奇，不依赖 AbilityTask 的异步激活流程。UGlyphInventoryComponent 封装了 PlayChargeMontage 和 StopChargeMontage 函数，分别负责蓄力蒙太奇的播放和停止。这些函数获取角色身上的 UAnimInstance 引用，直接调用 Montage_Play 和 Montage_Stop，播放时机由 TickComponent 中以绝对时间差驱动的蓄力逻辑精确控制。此方式绕过了 AbilityTask 的异步注册机制，消除了首次播放时的资源加载时序问题，使蓄力动画能够在第一次触发时即正确播放并循环。

### 9.4. 待确认事项

| 编号 | 事项 | 当前处理 | 需要确认的内容 |
|---|---|---|---|
| O-01 | 词条应用顺序 | 暂定“基础配置 → 等级 → 词条 → 变位 PreProcessContext → 蓄力倍率” | 是否按此顺序固定；确定后写入配置校验器 |
| O-02 | 现有 `Effect` 标签逐枚映射 | 行为标签采用 GDD 七类；`Range` → `Ranged` | 8 枚刻印中哪些 `Effect` 应为 `AOE`、`Heal`、`Summon` |
| O-03 | 融合主源选择交互 | 默认第一枚选中为主源 | 是否改为玩家显式选择主源/副源，或按槽位顺序确定 |
| O-04 | 变位槽完整表启用时间 | M3 先启用简化规则 | 完整五档表何时启用；是否随 Demo 层数调整 |
| O-05 | 多只 Boss 稍弱规则 | 初版单只 | 多只时的数量、生命倍率、共享信息素规则 |
| O-06 | 融合刻印词条继承 | 当前按主源保留、副源不保留 | 是否改为合并两枚词条、仅主源或仅副源 |
