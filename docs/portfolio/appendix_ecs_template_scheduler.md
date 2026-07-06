# Appendix: ECS System Template Scheduler Prototype

以下では、別プロジェクトとして作成した、ECS 向けの System Template Scheduler Prototype について説明する．

実装は次のリンクから確認できる．  
https://github.com/MoryYamat/TemplateScheduler


## もくじ
- [設計背景](#設計背景)
- [目的](#目的)
- [実現したこと](#実現したこと)
- [全体の構造と使用イメージ](#全体の構造と使用イメージ)
- [課題](#課題)

## 設計背景

ECS の System 実装において、  
実装量が増えるにつれて、**System の実行順や依存関係の管理**が課題になった．


例えば、以下のようにコード間にどのような関係があるかが不明瞭になることが問題であった．
```cpp
// intent
RunMovementIntentResolve(world, ctx);
RunPlayerCameraInput(world, ctx);

// intent resolve:
RunPlayerAttackIntentResolve(world, ctx);
// state
RunPlayerLocomotionState(world, ctx);
RunPlayerAttackState(world, ctx);
```
ここでは、 System 実行順が意図的なものなのかが不明である．  

そこで、**System 間にどのような依存関係があるかを明示的に扱う枠組み**があればよいと考えた．


## 目的

この Prototype の設計方針は以下のようにした．
- read / write などの 型レベル Access Effect から System 間の依存関係を構成する
- 依存関係をもとに、並列実行可能な System の候補を計算する

これにより、依存関係を明示的に扱う仕組みを作成することを目指した．

## 実現したこと
- 型レベルで System 間の依存関係を記述できる Compile-time Graph を実装
- read / write などの 型レベル Access Effect から 依存関係を構成する仕組みを試作
- 依存関係 Graph から、実行順序や並列実行可能な Layer を計算する方向性を検証
- 既存 System に専用の基底クラス継承を要求しない、非侵襲的な設計を試した


## 全体の構造と使用イメージ

実装は、System 間の依存関係を直接実行順として書くのではなく、  
まず型レベルの Graph として表現し、それを Runtime 実行のための Plan へ変換する構造を目指した．

全体の流れは以下のようになる．
```
Graph           : System 間の関係を表す構造
↓
IR              : Graph を解釈しやすい形に正規化した中間表現
↓
Interpretation  : edge が依存、順序、read/write conflict など何を意味するかを定義
↓
Plan            : 実行順序や並列実行可能 Layer として表した Scheduling 結果
↓
Executor        : Plan を runtime で実行する層
```

### 使用イメージ


#### 1. 概要
例えば、 System 間の依存関係を次のように型として記述する

```cpp
using SystemGraph = Graph<
                    Arc<InputSystem, IntentResolveSystem>,
                    Arc<IntentResolveSystem, MovementSystem>,
                    Arc<MovementSystem, AnimationSystem>,
                    Arc<AnimationSystem, RenderSubmitSystem>
                >;
```

この定義は、実行順そのものを手続き的に書くのではなく、  
System 間にどのような関係があるかを構造として表す．

この Graph を解釈し、実行順序や並列実行可能な単位へ変換する．

```cpp
using Plan = BuildPlan<SystemGraph>;
```

実行時には、この Plan に基づいて System を呼び出すことを想定している．

```cpp
Executor<Plan>::Run(world, ctx);
```

#### 2. 並列実行計画の作成
また、以下のように、型レベルの Access Effect System のようなものを記述することで、  
並列実行計画をコンパイル時に作成することができる。

まずは、ユーザが定義した、System について、reads / writes する型を以下のように列挙する

```cpp
// Movement_System は ユーザ定義のSystem型
// Effects<>は TemplateScheduler の型で、ユーザ定義型を用いて明示特殊化する
template<>
struct Effects<Movement_System>
{
    using reads = ResourcePack<IntentComponent, VelocityComponent>;
    using writes = ResourcePack<PositionComponent>;
};

template<>
struct Effects<CollisionDetection_System>
{
    using reads = ResourcePack<PositionComponent, CollisionComponent>;
    using writes = Resource<CollisionResult>;
};
```

次に、ExecutionSet として以下のように登録する

```cpp
using EXECUTION_SET = ExecutionSet<Movement_System, CollisionDetection_System>;
```

最後に、以下のように並列実行計画を作成し、実行することができる。
```cpp
using SAFE_LAYERED_PLAN = typename MakeSafeLayeredPlan<EXECUTION_SET>::type;

// 並列実行可能なシステムは非同期実行される (内部で std::async を使用)
ExecutePlanAsync<SAFE_LAYERED_PLAN>::Run(ctx);
```


## 課題

### 実用例との接続
現時点では、Template Scheduler は設計実験としての性質が強い．

そのため、実際の ECS System 実行に組み込んだ時に、どの程度有用かを検証する必要がある．

特に、現行実装のように  
`RunMovementIntentResolve` や `RunPlayerAttackState` などのSystem が増えた場合に、  
依存関係の定義が本当に読みやすくなるか、手動実行順より保守しやすくなるのかを確認する必要がある．

### 抽象化コスト
この Prototype は、Template を使った compile-time 計算に大きく依存している．

そのため、依存関係を型レベルで扱える一方で、実装の可読性、エラー追跡、コンパイル時間、保守性が課題になる．

特に、Graph や Access Effect を直感的に定義できない場合、  
実行順管理の問題を解決する代わりに、別の複雑さを導入してしまう可能性がある．

### 並列実行との接続

Template Scheduler は、compile-time に依存関係や実行 Layer を計算することを主な対象としている．

一方で、実際に並列実行を行うには、   
runtime 側の Task Scheduler、Thread Pool、同期処理、lifetime 処理などとの接続が、必要になる．

特に、 read / write conflict をもとに並列実行可能な System を計算できたとしても、  
実際の runtime で安全に実行するためには、  
Component の寿命管理、structural change、command buffer なども合わせて整理する必要がある．
