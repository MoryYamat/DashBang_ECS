# Roadmap

ここでは、現行実装の課題や今後の拡張方針について説明する．

## 現行実装の課題

### 1. ゲーム実装からのフィードバック不足

現行実装では、Engine 基盤の検証を目的とした最小構成の Gameplay を実装している．

一方で、ゲーム本編としての面白さ、レベル設計、敵、AI、ゲームループ、UI、演出などに関する設計や実装は限定的である．

Engine は、ゲームの楽しさを実現するための基盤であるべきだと考えている．  

したがって、今後は実際のゲーム実装を通じて、現行設計が有効な範囲を検証する必要がある．

### 2. Input Process Pipeline

現行実装は、以下のように構造設計を行っている．  

```txt
Raw Input
    ↓
Action Input (Abstracted Input)
    ↓
Requested Intent
    ↓
State / Resolver
    ↓
Resolved Intent
    ↓
Logic
    ↓
Render
```

この構造の主な目標は、AI / Network Input を統合することである．  

しかし、AI / Network Input に関する実装知見が少なく、制約理解があいまいであるため、  
課題や現実的な改善を考えられていない．

また、Animation Blend のための Logic -> Rendering 間のフィードバックループを考慮していない．  
現代的なゲーム表現や体験構築のための枠組みを整理する必要がある．

特に以下のことが問題である．

- AI Input / Network Input をどのように Requested Intent へ変換するのか
- 入力源に固有の特性とその違いを State / Resolver で吸収できるのか
- 各 Stage 間のフィードバックループの扱い

### 3. State / Resolver の構造的ボトルネック

現行実装は、Requested Intent を State / Resolver を通して、Gameplay Logic へ渡す構造にしている．

この構造によって、入力要求と状態制約、動作を分離しやすくなった一方で、  
規模が大きくなると、State-Machine の状態数や Resolver 責務が増大・複雑化する可能性がある．

今後は以下の点を考える必要がある．
- State の状態爆発を抑える方法
- Resolver の複雑性をどのように管理し、ハンドリング可能にするか
- Data / Authoring 側へ移すべき定義と、C++ code に残すべき処理をどう分けるか


### 4. Editor / Authoring 層

現在、Runtime の設計と動作を重視しており、Editor / Tool から編集する仕組みは十分ではない．  

Asset定義、Animation Bind、Hitbox などの Actor 定義を代表として、  
Terrain や Scene などの大きな時空間におけるデータの定義や編集についても考えていく必要がある．


## 今後の改善方針

### 1. ゲームプロトタイプの拡張

Engine 側の抽象設計を進めるばかりでなく、ゲームプロトタイプを拡張し、実際の Gameplay から設計の妥当性を検討する．  

特に以下の要素は重要である．  
- Enemy AI
- Action の試行錯誤のしやすさ
- VFX / SFX / UI との連携
- Game Rule 設計のためのコード量や変更箇所

### 2. AI / Network Input に関する Intent Pipeline の検証

まずは、AI Input の最小実装によってPlayer Input と AI Input が同じ Intent Pipeline によって動作するか検証する．  

また、個別事例に特有の制約に対して対応できるかについての検証も必要である．

これによって、現在の Input Process Pipeline が Local Player 専用の構造になっていないかを確認する．

### 3. State / Resolver の分割とデータ駆動化

State / Resolver が肥大化しすぎないよう、各制御ドメインに分割したり、優先順位制御の導入を検討する．  

将来的には、State の外部ファイルによる定義や、Visualization、Validation などを Editor 機能の拡張とともに検討する．

## 開発プロセスの改善

開発プロセスについても、以下のような要素を強化する必要があると感じている．  


### 1. パフォーマンス測定

現行では、設計と動作確認を優先して、パフォーマンス測定や比較ができていない．  

今後は、簡易的な測定を行いながらプロセスの確立を目指す．
- ECS Query / View の走査時間
- Component Storage のアクセス
- Animation Pose 更新の処理時間
- RenderCommand 生成の処理時間
- Asset load / build の処理時間

`std::chrono`による簡易測定から始め、Google benchmark などのツール利用などを検討していきたい．

### 2. テスト

現行実装では、Debug 表示やデモ実行による確認を中心に開発している．  
自動テストや単体テスト、境界テストを構築していくことが重要である．  

特に以下のような要素は優先的にテストが必要だと考えている．

- ECS generation / handle validation
- Query / View の select / require / exclude の条件判定
- AssetID / vpath / load state の管理やエラーハンドリング
- ImportData から Runtime Resource への変換結果の検証
- Transform / Animation Sampling などの基礎計算処理の妥当性
- ポインタ安全性や並列処理について

特にECS、Asset Pipeline は 単体で検証しやすい部分が多いと思われる．  

そのため、まず基本部分に小さいテストを追加しつつ、  
内部実装の変更時、破壊的変更を検出できるような構成にすることを目指したい．

### 3. 最小プロトタイプによる検証
これは個人的な開発手法に関する課題でもある．

自分のプロセスは、抽象的な設計や枠組みが先行し、実装に時間がかかるだけでなく、  
実装しないとわからない不確実性に対処するコストの見積もりがとても甘いという問題がある．

そのため、最初から大きく仕組みを作る前に、最小プロトタイプや限定されたケースで動作を確認し、  
必要性と問題点を把握してから本実装へ進めることを重視していく必要がある．

特に以下のような要素は最小プロトタイプによる確認が重要だと考えている．
- AI Input を Requested Intent へ接続する
- State / Resolver の優先順位制御の手法を検証する
- 外部 Animation Asset を既存 Skeleton に対応付ける
- Animation Blend

これにより、設計の失敗を早期に発見し、手戻りや不確実性に関するコストを減らすことができる開発プロセスを身に着ける必要がある．