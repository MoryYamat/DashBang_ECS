# 

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
- `TryGet()`で取得したアドレスは不安定であるため、取得したら即座に使用することによって一時対策を行う
- 非同期ロード・大量のロードが発生するような場合別の構造的対策が必要

## Portfolio Documents

このプロジェクトの設計意図や実装内容は、以下の資料にまとめています．

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