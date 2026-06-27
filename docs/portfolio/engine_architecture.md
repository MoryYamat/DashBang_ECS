# Engine Architecture

ここでは、Engine 基盤の構造と App(Game) 実装の構造を説明する．

## 目的
この Engine Architecture は、複数ゲームで再利用できる runtime 基盤 と App 固有の actor 定義や入力解釈などを分離することを目的としている．

これによって、Engine 側は、 ECS、AssetManager、Rendering、DebugDraw などの汎用機能を提供し、  
App 側はそれらを組み合わせて個別の Gameplay を記述できる構造にする．

## 設計方針
主な設計方針は以下の通りである．

### Engine側
- 個別 App に固有の定義や解釈・ルールの処理を含めない
- 各モジュール(システム)の分離を重視する
- runtime での効率性とAPIのユーザビリティを意識した設計にする

### App
- Engine 側のAPIを用いつつ、固有の処理やルールを構成していく
- App 側での実装でEngine 側に抽象化できる部分を明確にするためにできるだけ、実装構造を整理する

## 全体構造
全体構造は以下のようになっている．  

App側はEngineの機能を組み合わせて実装されている．
```
Engine:
    Platform / Window
    Input Abstraction
    File IO / Virtual Path
    Import Pipeline
    Asset Management
    ECS Runtime
    Math
    Animation Runtime
    Rendering / Camera
    Primitive Collision
    DebugDraw

App(Game):
    Asset Definitions
    Actor Definitions
    Input Mapping
    Intent Generation
    Player / Camera Control
    Actor / Attack State
    Gameplay Systems
    Animation Selection
    Game Loop Composition
```

## 主要モジュール
Engine 側の各 Subsystem は Game 固有の意味を持たない runtime 機能として分離する．

| Subsystem | Responsibility |
|-|-|
|Platform / Window|Window / Context生成、frame loopの基盤|
|Input Abstraction|device input を Action Inputとして抽象化|
|File IO / Virtual Path| `res://`などのvirtual path を実ファイルパスへ解決|
|Import Pipeline| glb などの外部データを ImportData へ変換する|
|Asset Management| AssetID、ロード状態、AssetStore への登録を管理|
|ECS Runtime|Entity / Component / System の実行基盤|
|Rendering / Camera| RenderCommand、Camera、GPU Resource を扱う|
|Primitive Collision| Hitbox / Hurtbox などの汎用形状判定|
|DebugDraw| runtime 状態を可視化するための debug rendering|

### App

App側では、Engine の Subsystem を組み合わせて、このデモ固有の Gameplay を構成する．

| Area | Responsibility |
|-|-|
|Asset Definitions| 使用する model / shader / animation clip の定義|
|Actor Definitions| Paladin などの actor 初期化と component 構成|
|Input Mapping | 入力を Action として解釈するための定義|
|Intent Generation | Action Input から actor / camera への intent を生成|
|Actor / Attack State| actor の移動状態や攻撃状態を管理|
|Gameplay Systems | 攻撃、hitbox spawn、animation selection などの game 固有処理|
|Game Loop Componsition | Engine / App system の実行順序を構成|

## Engine / App の境界

Engine 側には、Game固有の意味を持たない汎用機能を置く．  
App側には、その機能をどのような Gameplay として解釈するかを置く．

例として、 Hitbox/Hurtbox では次のように分離をしている．

```
Engine:
    HitboxComponent
    HurtboxComponent
    LifetimeComponent
    Primitive Collision
    HitboxHitEvent

App:
    AttackState
    AttackDefinition
    AttackHitboxSpawnSystem
    HitEvent の Gameplay 解釈
```

## この構造にした理由

### Game 固有の意味を Engine に混ぜないため
Engine 側は、 ECS、Asset、Rendering、Animation、Primitive Collision などの汎用的な runtime mechanism を提供する．  
一方で、入力をどのような操作意図として解釈するか、Actor がどの状態を持つかなどは App 固有の意味である．

そのため、Engine 側には「汎用的に実行できる処理」を置き、App 側には「それをこの Game でどう解釈するか」を置く構造にした．

### 変更の影響範囲を局所化するため

初期実装では、 Manager や Importer などに責務が集中し、内部実装の変更が外部へ波及しやすかった．  
現行実装では、ECS、Asset、Import、Rendering、Debug などを 分離することで、  
Storage 戦略、Loader、Rendering backend などの変更が他の層へ広がりにくい構造を目指した．

### App 実装を通して Engine 側へ抽象化すべきものを見極めるため
最初からすべてを Engine 側へ抽象化すると、 Game 固有の処理まで Engine 側に入り込みやすい．  
そのため、まず App 側で具体的な Player、Camera、Attack、Hitbox Spawn などを実装し、  
その中で、複数の Game に共通しうる仕組みを Engine 側へ分離する方針にした．

## 現行実装の課題

### 実行順序と依存関係の問題
現行実装では、Engine / App の system を手動で組み合わせて Gameplay を構成している．  
そのため、入力処理、状態更新、物理的な移動、Animation、Renderingなどの実行順序が重要になる．  

今後は、system 実行順序や依存関係をより明示的に整理し、変更時に追いやすい構造にする必要がある．

### Runtime / Authoring / Tool / App 層の境界整理

現行実装では、actor 定義、asset 定義、状態定義、system 構成の多くを C++ コード上で記述している．  
そのため、構造は確認しやすい一方で、データ駆動化や editor / tool からの編集にはまだ対応していない．  

今後、Engine をより汎用的な runtime 基盤として発展させるためには、App側で書かれている定義情報を  
どの範囲まで runtime 機能として扱い、どの範囲を Authoring / tool 側へ移すかを検討する必要がある．


### Subsystem 間の依存関係とテスト
ECS、Asset、Import、Rendering などの Subsystem は分離したが、境界条件や寿命管理に対するテストは十分でない．  
特に Entity の generation、AssetID、Resource pointer の寿命、system 実行順序、frame lifetime を持つデータの扱い  
などは今後テストを追加し、保守性と堅牢性を高める必要がある．