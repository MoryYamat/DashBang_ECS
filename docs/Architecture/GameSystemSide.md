## **Skill System Data Flow**

### **システムの概要**
|層|内容|
|:-|:-|
|入力層|ハードウェア入力を抽象化(`RawInputState`->`InputActionComponent`)|
|マッピング層|抽象入力->スキルIDの対応(`SkillInputMap`)|
|トリガー層|入力に応じてスキルエンティティを生成|
|発動処理層|`ActiveSkillComponent`に基づいて攻撃範囲などを生成(`SkillCastingSystem`)|
|終了処理層|スキル寿命やクールダウンを処理(今後実装予定)|


### **処理フロー**
```text
[InputAction] -> [SkillSlot] <- [SkillID] <- [SkillDefinition]
                    ↓ (SkillTriggerSystem)
            [SkillInstanceComponent] (<=create)
                    ↓ 
            [SkillCastingSystem] (=>attack generation)
                    ↓
            [SkillLifetimeSystem] (=>cleanup)
```

* データと処理の分離
* 汎用的なトリガー設計
* データ駆動設計 `SkillInputMap`による柔軟なスキルバインドの実装
* 処理レイヤの分離 入力・発動・定義・終了が完全に責務分離している


### **将来的な拡張**
* 「状態をデータの存在で管理する」ように設計を変更する
> `PlayerCharacterActor`が`ActiveSkillComponent`を持っており，キャラクター自身がスキル状態をまとめて持っている形になっている．
* `ActiveSkillComponent`の存在自体が発動中を示すように設計変更する