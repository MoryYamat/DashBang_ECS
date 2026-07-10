# DDKND: ECS-based Game Engine Runtime Prototype

Readme.md の 言語  
| [English](Readme.md) | [日本語](Readme.ja.md) |


## Portfolio Documents

このプロジェクトの設計意図や実装内容は、以下の資料にまとめています．

[デモ動画(Youtube)](https://youtu.be/XLh3T2ZbSMs)

これらの資料はすべて日本語です．

**関心のある技術領域に応じた閲覧順は、[リーディングガイド](/docs/guides/reading_guide.md)を参照してください!**


- [Overview](docs/portfolio/overview.md)  
プロジェクトの設計と構造の概要を説明
- [Engine Architecture](docs/portfolio/engine_architecture.md)  
runtime のレイヤー構造と Engine / App の分離を説明
- [System Pipeline](docs/portfolio/system_pipeline.md)  
input->intent->state->logic->rendering の流れを説明
- [ECS](docs/portfolio/ecs.md)  
Entity / Registry / Storage / View / Query の設計を説明
- [Asset Manager](docs/portfolio/asset_manager.md)  
AssetManager / AssetStorage / AssetStore / Loader の分離を説明
- [Import Pipeline](docs/portfolio/import_pipeline.md)  
Importer / ImportData / RuntimeResource の分離を説明する
- [Lessons Learned](docs/portfolio/lessons_learned.md)  
初期実装の課題と、現行実装への改善内容を説明
- [Roadmap](docs/portfolio/roadmap.md)  
現行実装の制限・課題と将来の拡張を整理

### Appendix
- [Debug Tools](docs/portfolio/appendix_debug_tools.md)  
DebugDraw, DebugCamera, Skeleton, Hitbox/HurtBox, HitEvent 表示を説明
- [FSM Engine](docs/portfolio/appendix_fsm_engine.md)  
過去に独自実装した FSM-Engineについて、設計背景、データ駆動構造、ECSとの統合方法、現行実装へ採用しなかった理由を説明
- [ECS-System Template Scheduler](docs/portfolio/appendix_ecs_template_scheduler.md)  
独自実装した ECS の System 実行順序をCompile時に解決する非侵襲的なライブラリの説明
- [Quality Checks and Improvements](docs/portfolio/appendix_quality.md)  
ポートフォリオに対して行ったテスト、静的解析、メモリリーク確認、改善方針を説明

## 動作環境
- OS: Windows11
- 

## 使用ライブラリ
- 

## 基本構造
raw input -> Requested intent -> Resolver(FSM) -> ResolvedIntent -> UpdateLogic -> UpdateRendering

### IntentRequest
ユーザインプット・AI・ネットワークなどから`RequestedIntent`を作成

### State
`RequestedIntent`・`Environment`・`CurrentState`から ActorState/FSM を更新

### IntentResolve
`RequestedIntent`と`State`から`AcceptedIntent`を作成

### ロジック
`AcceptedIntent`と`Stats`から`GamePlayState`を更新する


### 例
camera
```
RawInput
↓
ActionInput
↓
PlayerControllerEntity

RequestedCameraIntentComponent
↓
CameraRigEntity
↓
CameraDesiredPoseComponent

CameraEntity
↓
Transform/CameraLook/CameraMatrices
```

## 確認されている問題点

### AssetStoreのコンテナであるAssetStorage::vector<T>の動的な再割り当てによって発生するアドレスの不安定性に関連する問題。
- 問題は、`AssetStore::TryGet(AssetID<T>)` で取得した `Resource*`(T*) のアドレスが、内部コンテナ (`vector<T>` など) に依存していること
- `TryGet()`で取得したアドレスは不安定であるため、**取得したら即座に使用すること** によって一時対策を行う
- 非同期ロード・大量のロードが発生するような場合別の構造的対策が必要

## 古い実装

以下のリポジトリは、本リポジトリの古い実装である．

- https://github.com/MoryYamat/Dashbang_ECS_OLD

