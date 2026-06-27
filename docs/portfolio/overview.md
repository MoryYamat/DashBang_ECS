# Overview

ここでは、現行実装を概説する.

## 目的
このプロジェクトは、ECSベースの game engine runtime architecture prototype  の作成を目的とした．

ECSアーキテクチャをベースとして、Game Engineの runtime における基盤機能を提供するデータおよびデータフローの設計と実装を行った．  

以下では、このプロジェクトの成果と全体構造、各詳細部の説明資料へのリンクを掲載する．

## プロジェクトの成果
このEngine基盤を使用して、最小構成のゲームアプリケーションを実装した．  

プレイヤー操作、カメラ制御、スキンメッシュアニメーション、攻撃 hitbox / hurtbox 判定、Debug 描画までを接続し、  
runtime 基盤として一連のゲームプレイ処理を動作確認した．

[デモ動画](youtube link)

具体的には、以下のようなことが実現できている
- インポートした3Dモデルの スキンメッシュアニメーション の再生
- ユーザのキーボード・マウス入力に基づいたプレイヤーキャラクターとカメラの操作
- 攻撃ボタン入力から、攻撃アニメーション再生、hitboxによる当たり判定の実行
- デバッグ描画による FSP、Skeleton、Hitbox / Hurtbox、HitEvent の可視化

## 全体構造の概要
ここでは、
- ユーザ入力の処理パイプライン
- Engine と App の 構成

について説明する


### ユーザ入力の処理パイプライン
ユーザ入力はデバイス入力から Action Input として抽象化される．  
Action Input は Intent (操作意図)として Actor などに 状態変化 をリクエストする入力として作用する．  
Actorなどのゲーム状態はこれらの Intent を解決し GamePlayとして反映されたり、Renderingされたりする．  
```
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
GamePlay Logic
    ↓
Rendering / Debug Visualization
```

### Engine と App の 構成
Engine 側には複数ゲームで再利用できる runtime 機能を置き、App 側にはこのデモ固有の actor 定義・入力解釈・状態遷移・ゲームルールを置く．

```
Engine:
    ECS
    Asset Management
    Import Piepline
    Animation Runtime
    Rendering
    Hitbox / Hurtbox primitive Collision
    DebugDraw

App:
    Input Mapping
    Actor Definition
    Player / Camera Control
    Intent Generation
    Actor State 
    Gameplay Rule Interpretation
    Animation Selection
```

## ドキュメント

- [Engine Architecture](./engine_architecture.md)  
runtime のレイヤー構造と Engine / App の分離を説明
- [System Pipeline](./system_pipeline.md)  
input->intent->state->logic->rendering の流れを説明
- [ECS](./ecs.md)  
Entity / Registry / Storage / View / Query の設計を説明
- [Asset Manager](./asset_manager.md)  
AssetManager / AssetStorage / AssetStore / Loader の分離を説明
- [Import Pipeline](./import_pipeline.md)  
Importer / ImportData / RuntimeResource の分離を説明する
- [Lessons Learned](./lessons_learned.md)  
初期実装の課題と、現行実装への改善内容を説明
- [Roadmap](./roadmap.md)  
現行実装の制限・課題と将来の拡張を整理

### Appendix
- [Debug Tools](./appendix_debug_tools.md)  
DebugDraw, DebugCamera, Skeleton, Hitbox/HurtBox, HitEvent 表示を説明
- [FSM Engine](./appendix_fsm_engine.md)  
現行実装には使用していない、独自実装した FSM-Engine の設計と実装を説明
- [ECS-System Template Scheduler](./appendix_ecs_template_scheduler.md)  
独自実装した ECS の System 実行順序をCompile時に解決する非侵襲的なライブラリの説明
