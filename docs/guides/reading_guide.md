# Reading Guide リーディングガイド

ここでは、把握したい内容によってドキュメントを順番に提示しています。

## 一覧
- [A. 設計の全体像を把握したい場合](#a-設計の全体像を把握したい場合)
- [B. 各システムの設計と改善の過程を把握したい場合](#b-各システムの設計と改善の過程を把握したい場合)
- [C. FSM-Engine について知りたい場合](#c-fsm-engine-について知りたい場合)
- [D. TemplateScheduler について知りたい場合](#d-templatescheduler-について知りたい場合)

## A. 設計の全体像を把握したい場合

1. [Overview](/docs/portfolio/overview.md)  
プロジェクトの設計と構造の概要を説明
2. [Engine Architecture](/docs/portfolio/engine_architecture.md)  
runtime のレイヤー構造と Engine / App の分離を説明
3. [System Pipeline](/docs/portfolio/system_pipeline.md)  
input->intent->state->logic->rendering の流れを説明
4. [Roadmap](/docs/portfolio/roadmap.md)  
現行実装の制限・課題と将来の拡張を整理

### Appendix
5. [Quality Checks and Improvements](/docs/portfolio/appendix_quality.md)  
ポートフォリオに対して行ったテスト、静的解析、メモリリーク確認、改善方針を説明
6. [Lessons Learned](/docs/portfolio/lessons_learned.md)  
初期実装の課題と、現行実装への改善内容を説明


## B. 各システムの設計と改善の過程を把握したい場合

1. [Overview](/docs/portfolio/overview.md)  
プロジェクトの設計と構造の概要を説明
2. [ECS](/docs/portfolio/ecs.md)  
Entity / Registry / Storage / View / Query の設計を説明
3. [Asset Manager](/docs/portfolio/asset_manager.md)  
AssetManager / AssetStorage / AssetStore / Loader の分離を説明
4. [Import Pipeline](/docs/portfolio/import_pipeline.md)  
Importer / ImportData / RuntimeResource の分離を説明する
5. [Lessons Learned](/docs/portfolio/lessons_learned.md)  
初期実装の課題と、現行実装への改善内容を説明

### Appendix
6. [Debug Tools](/docs/portfolio/appendix_debug_tools.md)  
DebugDraw, DebugCamera, Skeleton, Hitbox/HurtBox, HitEvent 表示を説明
7. [Quality Checks and Improvements](/docs/portfolio/appendix_quality.md)  
ポートフォリオに対して行ったテスト、静的解析、メモリリーク確認、改善方針を説明

## C. FSM-Engine について知りたい場合

1. [Overview](/docs/portfolio/overview.md)  
プロジェクトの設計と構造の概要を説明
2. [Engine Architecture](/docs/portfolio/engine_architecture.md)  
runtime のレイヤー構造と Engine / App の分離を説明
3. [System Pipeline](/docs/portfolio/system_pipeline.md)  
input->intent->state->logic->rendering の流れを説明
4. [FSM Engine](/docs/portfolio/appendix_fsm_engine.md)  
過去に独自実装した FSM-Engineについて、設計背景、データ駆動構造、  
ECSとの統合方法、現行実装へ採用しなかった理由を説明

## D. TemplateScheduler について知りたい場合

1. [Overview](/docs/portfolio/overview.md)  
プロジェクトの設計と構造の概要を説明
2. [System Pipeline](/docs/portfolio/system_pipeline.md)  
input->intent->state->logic->rendering の流れを説明
3. [ECS](/docs/portfolio/ecs.md)  
Entity / Registry / Storage / View / Query の設計を説明
4. [ECS-System Template Scheduler](/docs/portfolio/appendix_ecs_template_scheduler.md)  
独自実装した ECS の System 実行順序をCompile時に解決する非侵襲的なライブラリの説明