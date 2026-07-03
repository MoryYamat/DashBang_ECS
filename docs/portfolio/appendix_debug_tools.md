# Appendix: Debug Tools

## 目的
runtime の状態を確認しながら実装を進めるために、最低限の Debug 表示を用意した．

## 実現したこと
- FPS 表示
- Skeleton Debug Line
- Hitbox / Hurtbox の Debug 描画
- HitEvent の表示

## 利用目的
Skinned Mesh Animation、glTF import、Hitbox / Hurtbox Collision など、  
内部状態が見えにくい処理の検証に使用した．


## 現行実装の課題
- Debug 表示のカテゴリ化は未整理
- Debug Menu / Editor との連携は未実装
- HitEvent などの履歴表示は最低限の実装