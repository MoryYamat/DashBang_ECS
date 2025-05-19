## **Skill System**

### **システムの概要**
> 入力（InputAction）→ スロット（SkillSlot）→ スキル（SkillId）→ 発動（Entity）
という段階的解決を経て、スキルの即時発動が行われる。


### **処理フロー**
```text
[InputAction] ──▶        [SkillSlot]
       ▲                       │
       │                       ▼
[InputActionComponent]  [SkillSlotAssignmentComponent]
       │                  │
       └────── SkillInputMap ───┐
                                ▼
                       [SkillTriggerSystem]
                                │
                                ▼
                     Create [SkillInstanceComponent]
                                │
                                ▼
                     [SkillCastingSystem]
                                │
                                ▼
                     Generate Attack2DArea
                                │
                                ▼
                     [SkillLifetimeSystem]
                                │
                                ▼
                  Auto-destroy expired entities

```
#### **各ステージの説明**
|ステージ|	内容|
|:-|:-|
|InputAction|	GLFWなどによる生の入力定義（マウス・キーボード）|
|SkillInputMap|	InputAction → SkillSlot のマッピング（バインディング）|
|InputActionComponent|	各プレイヤーが今押しているアクション（フレームごとに更新）|
|SkillSlotAssignmentComponent|	各プレイヤーに対応する SkillSlot → SkillId 割り当て情報|
|SkillTriggerSystem|	押されている InputAction に基づき、プレイヤーのスロット → スキルID を引いてスキル発動をトリガー（エンティティ生成）|
|SkillInstanceComponent|	スキルが発動中であることを記録するエンティティコンポーネント|
|SkillCastingSystem|	SkillDefinition を取得し、実際の攻撃判定などを生成|
|SkillLifetimeSystem|経過時間を監視し、スキルエンティティを削除する|

### **設計の特徴**
* データと処理の分離
* 汎用的なトリガー設計
* データ駆動設計 `SkillInputMap`による柔軟なスキルバインドの実装
* 処理レイヤの分離 入力・発動・定義・終了が完全に責務分離している


### **将来的な拡張**
* 攻撃範囲の動的な変化・移動（移動する攻撃、追尾、広がる範囲など）
* 複合形状スキル(例：範囲+ビームなど)
* キャラクター状態との同期，ヒットイベント，ステータス変化，クールダウン管理などの追加