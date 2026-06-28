# ECS

ここでは、現行実装における Entity Component System の構造と、初期実装からの改善点について説明する．

## もくじ
- [目的](#目的)
- [実現したこと](#実現したこと)
- [現行実装の全体構造](#現行実装の全体構造)
- [主要要素](#主要要素)
- [初期実装の課題](#初期実装の課題)
- [改善方針](#改善方針)
- [改善結果](#改善結果)
- [現行実装の課題](#現行実装の課題)

## 目的

この ECS 実装は、GameObject の継承構造に依存せず、  
Entity に対して必要な Component を組み合わせることで、  
game runtime の状態を柔軟に構成できるようにすることを目指した．

自作 Engine を学習しながら拡張していく過程では、  
最初からすべての機能や責務を見通すことが難しい．  

そこで、(自分としては) 継承階層に機能を固定していくよりも、  
Component と System を追加・差し替えながら機能を拡張できる構造のほうが適していると考えた．

また、機能追加や内部実装の変更が他の機能へ波及しにくい構造にすることを重視した．  

そのため、Entity 管理、Component Storage、Query / View、World API の責務を分離し、  
System 側が必要な Component の組み合わせを宣言して処理対象を取得できる構造を目指した．

## 実現したこと
- Entity の生成 / 破棄
- Component の追加 / 削除 / 取得
- Generation による Entity handle の安全性確保
- Sparse Set による Component Storage
- Query / View による Entity Filtering
- select / require / exclude による System 側からの対象指定

## 現行実装の全体構造

- World
    - Registry を保持する薄い orchestrator
    - 外側から ECS へアクセスする入り口

- Registry
    - Entity の生成 / 破棄
    - Component Storage の管理
    - Component の追加 / 削除 / 取得
    - Query から View を生成

- Storage<T>
    - Component 型ごとの実データを保持
    - `components_` / `ents_` / `sparse_` による sparse set 構造

- Query
    - `select` / `require` / `exclude` による条件定義
    - World / Registry に依存しない

- View
    - Query を Registry に bind して Storage を走査

## 主要要素

### Entity
現行実装では、Entity を `Generation + Index`で表現している．  
`Index`は Entityの識別 と Storage への対応に使用する．  
`Generation` は破棄済み Entity への古い参照を検出するために使用する．

主な責務:
- `Index`により Entity と対応する Data / Component の位置を識別する
- `Generation` により 古い Handle の再利用を検出する
- `IsAlive`によって現在有効な Entity かを確認する

### Registry
Registry は、Entity 管理、Component Storage 管理、 Component CRUD、View 生成を担当する．

主な責務:
- Entity の生成 / 破棄
- Entity の Generation を管理
- `AddComponent` / `TryGetComponent` / `RemoveComponent` を提供
- `view(query)` によって Query を View に変換

### Storage
`Storage<T>`は、Component 型ごとの実データ、Entityとの対応を保持する．

主な責務:
- `components_` : 実データの連続配列(dense)
- `ents_`       : dense 側のindex に対応する Entity を保持
- `sparse_`     : Entity Index から dense index への対応保持

### Query / View
Query は System が必要とする、 **Component 条件を表すための型**である．  
- `select`      : 主走査 Component を表す
- `require`     : 必要な Component を表す
- `exclude`     : 不要な Component を表す

Query 自体は World / Registry に依存しない．  
実際の Storage 走査は、Query を Registry に bind して生成される View が担当する．

- `view`: Query 条件に従って Storage を走査し、Component参照を返す

使用例:
```cpp
auto view = registry.view(
            query().select<TransformComponent>()
                   .require<VelocityComponent>()
                   .exclude<DisabledComponent>());

for(auto [transform, velocity ] : view){...}
```

この分離により、System 側は Storage の内部構造を直接意識せず、  
必要な Componentの組み合わせを宣言して処理対象を取得できる．

### World
World は、Engine / App から ECS へアクセスするための入り口である．

主な責務:
- 外側から ECS へアクセスする入り口
- Registry の所有
- Entity の基本操作 API の提供
- 必要に応じて `GetRegistry()` から Registry を取得

以前の実装では、 World 相当のクラスが Entity 管理、 Component 管理、 Storage 管理、 View 生成など(のAPI)をまとめて持っていた．

これによって、責務が集中しやすくなり、変更の影響波及が局所化しづらくなっていた．

現行実装では、World は Registry を保持する薄い orchestrator として扱い、  
ECS の具体的な操作API は Registry 側に寄せている．

これにより、World を巨大な管理クラスにせず、  
ECSの内部構造を Registry / Storage / View 側へ分離できる．

## 初期実装の課題

初期実装では以下のような課題があった．

- `EntityManager` クラスに、現行実装で、`Registry` / `Storage` / `View` として分離されているコア機能が集中していた
- Component Storage に `std::shared_ptr<T>`を使用していたため、ECSの利点である、キャッシュ局所性を活用しづらくなっていた

`EntityManager` クラスに責務が集中していたことによって、  
ECSの実装を変更した場合の、修正箇所が増大することが予測され、  
技術的負債が蓄積する原因となっていた．

`std::shared_ptr<T>`は簡易実装には向いているが、Storage 戦略の変更が難しく、キャッシュ局所性による高速化メリットも得られにくい実装になっていた．

## 改善方針

現行の実装はこれらの課題を緩和することを意識して設計した．  
具体的には以下のことを重視した．

- 1つのクラスやシステムに責務が集中しないようにする
- `Storage<T>`クラスを作成し、キャッシュ局所性を活用しやすい実装に変更する．  
また、後から Storage 戦略 の変更を行いやすい設計にする．
- 使用側ではできるだけ直感的に利用できるAPIにする

## 改善結果

責務集中に関しては、少なくとも前回の実装よりは緩和できた．  
責務境界もはっきりしたので、内部実装の変更も行いやすくなった．

`Storage<T>` クラスを作成したことによって、Storage 戦略を 変更しやすくなった．
またその変更の影響波及も内部処理の変更によって対応することができるようになったと思われる．

## 現行実装の課題

### 1. Component 参照の寿命管理
現行実装では、`Storage<T>`のコンテナに`std::vector<T>`をそのまま使用している．  
これによって、`components_`が再確保された場合に、  
それまでに`TryGetComponent<T>`で取得されたポインタはダングリングポインタになる可能性がある．  

次に、削除時に `components_` の連続性を保つために、`swap & pop_back`による削除を行っている．  
これによって、Component の配置が変わる可能性 があり、長期間 Component へのポインタや参照を保持する設計とは相性が悪くなっている．

これによって、Component 参照の寿命管理や、将来的な並列実行時の安全性に影響をもたらすことが考えられる．

現在は一時対策として、取得したポインタを長期間保持せず、取得した処理内で即座に使用する方針にしている．

今後の対策としては以下のようなことが考えられる．
- Component Storage について、事前に`vector::reserve()`する
- Component への直接ポインタではなく、handle 経由で再取得する設計に変更する
- Component の追加 / 削除などの 構造変更操作 を frame 境界 や command として集約する設計に変更する