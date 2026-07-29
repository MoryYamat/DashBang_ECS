# DDKND: ECS-based Game Engine Runtime Prototype

Language:  
| [English](Readme.md) | [日本語](Readme.ja.md) |

## デモ

[デモ動画(Youtube)](https://youtu.be/XLh3T2ZbSMs)

DDKNDは、ECSを中心に構成したC++20製のゲームエンジン・ランタイム試作です。
ゲームエンジンのランタイム設計、データ指向の処理構成、GPUアニメーション、およびデバッグ機能の実装を目的として個人開発しました。

主な実装範囲：

- ECSのEntity、Storage、View、Query
- ゲームロジックのSystem Pipeline
- glTFアセットのインポートおよび管理
- GPUスキニングアニメーション
- Hitbox／Hurtboxとデバッグ描画
- CMakeによるビルド構成と自動テスト

設計上の判断、旧実装からの改善、現在の制限事項についても文書化しています。  
[リーディングガイド](/docs/guides/reading_guide.md)

### Features
- 100体／1000体のキャラクター描画
- プレイヤーキャラクターの移動と攻撃
- GPUスキニングアニメーション
- Hitboxおよびデバッグ情報の可視化


### デバッグ操作
| key | Function |
| --- | --- |
| F1 | フレーム時間情報切替 |
| F2 | Hitbox / Hurtbox の表示切替 |
| F3 | スケルトンの表示切替 |
| F4 | HitEvent 情報の表示切替 |
| F5 | デバッグカメラモードの切替 |

## Build
CMakeコマンドを使用してビルド可能です．

GameアプリのDebugビルド
```
cmake --preset x64-debug-game
cmake --build --preset x64-debug-game
```

GameアプリのReleaseビルド
```
cmake --preset x64-release-game
cmake --build --preset x64-release-game
```

ddkndエンジンのテストのビルドと実行
```
cmake --preset x64-debug-ddknd-tests
cmake --build --preset x64-debug-ddknd-tests
ctest --preset x64-debug-ddknd-tests
```

## 使用ライブラリ
- fmt
- spdlog
- GLFW
- GLAD
- cgltf
- stb

## 動作確認済みの環境
- Windows11
- C++20
- MSVC (cl.exe)
- CMake 3.25


## Portfolio Documents

このプロジェクトの設計意図や実装内容は、以下の資料にまとめています．

これらの資料はすべて日本語です．

**関心のある技術領域に応じた閲覧順は、[リーディングガイド](/docs/guides/reading_guide.md)を参照してください．**


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
Importer / ImportData / RuntimeResource の分離を説明
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


## 古い実装

以下のリポジトリでは、本プロジェクトの旧実装を公開しています．

- https://github.com/MoryYamat/Dashbang_ECS_OLD

