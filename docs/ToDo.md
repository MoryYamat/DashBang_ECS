# **TODO**

## Status Icons
* ✅: Completed(完了済み)
* 🚧: Processing(実装中)
* 🔜: Soon(近日中に対応予定)
* 📝: Planning(やる必要あり・保留中)
* ❓: Needs consideration(要検討)
* ❗: Important(重要)

> 備考：
> * 完了済みでも意味のあるタスクは取り消し線で履歴として残す
> * 上記ステータスは主に見た目と管理目的．実装順や優先度整理の助けとする

## 📌**System Design**
* 🚧[SkillFSMの実装](#skillfsm)
* 🔜[直交FSMの相互作用の実装](#直交fsm)
* 📝[直交FSMの統合管理機構の実装](#直交fsm管理)
* ❓[hitboxEffectのFSM化](#hitboxEffect)

<details>
<summary id="skillfsm"> <strong>🚧SkillFSMの実装 </strong> </summary>

#### **タスク**
* ~~`FSM`側で生成された`Entity`の振る舞いを実装する`ECS`側の`Sysmtem`を実装(`SpawnHitbox`/`LifetimeSystem`/`SkillTrajectorySystem`)~~
* ~~より複雑な`Lifetime`定義・管理方法の実装(`State::Active`と**同期/非同期**を選択可能に)~~
* `request-resolver`方式への移行
   * 状態遷移`priority`の定義方法の検討
* `Cancel`動作の実装のための設計

#### **背景・判断理由**
* **定義ドリブン**で柔軟なFSM実装には`switch(enum)`より`型タグ(struct)`が有用
* 状態ごとの責務分離を促進できるため，拡張が容易
* FSM側では定義から`Entity`の**依存(Component)**/**寿命** などを初期化して生成=>ECS側で管理

#### **課題・迷い**
* 多段遷移を考える必要がある場合，どうすれば多段遷移リクエストを送るような実装ができるか(自動リクエストチェーン(ステップドライブ))
* 削除条件が`State::None`に依存してよいのか検討中
* `EffectExecutionRecord`が単純なレコードになっているため，**同じ副作用の繰り返し**を処理できない問題
* `SpawnHitboxEffect.cpp`で`Transfrom2DComponent`の`scale`をハードコードしている点

#### **メモ・備考**
* `request-resolver`方式への移行は，直交FSM協調に有効そう
* 

</details>

<details>
<summary id="直交fsm"> <strong> 🔜直交FSMの相互作用の実装 </strong> </summary>

#### **タスク**
* 分析を深める

#### **背景・判断理由**
> 2つの要素がある
 * ① 状態への干渉：他FSMの状態によって，自FSMの状態遷移を誘発する(`FSMStateTransitionMediator`/`FSMTransitionRequestComponent`)
    * 例：`CCFSM`の状態により，`SkillFSM`の状態を変化させる(**副作用的に？**)
 * ② 振る舞いへの干渉：他FSMの状態に応じて，制御対象データに影響を与える(`CrossFSMEffectSystem`/`ModifierComponent`)
    * 例：`skillFSM`の状態により（Skill定義に基づいて），`movementFSM`の速度を変える

#### **課題・迷い**


#### **メモ・備考**
</details>

<details>
<summary id="直交fsm管理"> <strong> 📝直交FSMの統合管理の実装 </strong> </summary>

#### **タスク**

#### **背景・判断理由**

#### **課題・迷い**


#### **メモ・備考**
</details>

<details>
<summary id="GeneralizationofFSMsystems"> <strong>📝テンプレートを用いたFSMSystemの汎用インターフェース化 </strong> </summary>

#### タスク
* 

#### 背景・判断理由
* 

#### 課題・迷い
* 

#### メモ・備考
* 

</details>


<details>
<summary id="hitboxEffect"> <strong>❓HitboxEffectのFSM化 </strong> </summary>

#### タスク
* SkillFSMの副作用として生成される`hitboxEffect`のFSM化の検討

#### 背景・判断理由
* 高度な当たり判定の制御のためにFSM化する必要があるかどうか検討する
* アニメーションや多段制御などより手触り感をよくするために必要か．実装コストが高いため過剰設計にならないか要検討
* 

#### 課題・迷い
* **明示的に制御したい状態が，時間やイベントに応じて遷移する必要があるかどうか**が判断基準

#### メモ・備考
* 後回しでもよい．(**YAGNI**)

</details>