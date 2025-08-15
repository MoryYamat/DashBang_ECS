# <Design-notes FSMの状態干渉の構造分析・設計>
- **Stats**: Draft
- **Owner**: @you
- **Related ADR**:
- **Related Code/PR**: <Game/Character/FSM/*>

## Problem
- `CCFSM`(直交FSM)から`MovementFSM`/`SkillFSM`へ状態干渉を行う，安全で再利用可能な最小構造を定義したい
- 目標は単一状態原則(Single-state)を壊さず，決定性と自己回復性を確保すること

## Goals / Non-goals
- Goals: 3–5点（観測可能に書く）
    1. `CCFSM`がCC開始時，`SkillFSM`を1回だけ`Interrupted`へ遷移させる(ログで観測)
    2. `CCFSM`がCC開始時，`movementFSM`を強制停止(`Locked`推奨／最小構成では`IDLE`でも可)させる(在位中は維持)
    3. **制御プレーン(占有権／Lease)**の導入：`{id, severity=Hard, payload, ttl}`をターゲットFSM側で保持
    4. 再利用可能な構造の整理：採択は`.priority`，撤回は`id`(Release/TTL)で扱う．
- Non-goals: 今回やらないこと（スコープ外）
    - パフォーマンス最適化(スーパーセット・SoA移行など)
    - 汎用インターフェースの抽象化／DSL化
    - `Inhibit/Soft`などHard以外の干渉ポリシーの実装

## Context & Assumptions
* ECSベース／1フレーム内に単一スレッド更新(将来並列化の可能性あり)
* 各FSMは同時に1状態のみ(single-state)
* イベント／リクエストはフレーム教会で順序逆転・取りこぼしが起こりうるという前提で設計(自己回復が必要)
* 入力は`Intent`->`Resolver`->`FSM`->`Logic`のパイプラインを通る
* デバッグログは採択／解除／失効の可観測性を担保する

## Interfaces (Data Contracts)
### Components

### Events/Requests


### Side-effects keys

## Flow / Ordering (1フレームの順序)
`Lease更新 -> Control適用 -> 内部遷移解決 -> 副作用`<br>
* Lease更新：新規`Issue/Release`を処理しつつ`ttl-=dt`(0以下は自然失効)．
* Control適用(干渉)：
    * Movement:`active_lease && severity=Hard` -> 即`Locked`を出力(内部より上位)
    * Skill:新規受領フレームのみ`TransitionRequest(Interrupted, high)`を1回発行
    * Skill起動条件に`!has_active_lease`を追加(在位中は`None->Casting`禁止)．
* 内部遷移解決：`.priority > ...`で通常通り解決(データプレーン)．
* 副作用：解決結果に基づいてフック実行
* Queue寿命：`TransitionRequest`はframe-bounded，重複は優先度／同一宛てで抑止

## Invariants
常に守る性質（決定性・冪等・再入安全…）を3点以内。
- Single-state: 各FSMは常に1状態
- 所有権と寿命：各FSMの`ExternalAuthoritySlot`は最多1枚のLeaseを保持．`LeaseRelease(id)`はid一致時のみ有効．`ttl`は単調非増
- 決定性：`severity(層) > priority(同層内の優先度) > issued_at(発行時刻) > issuerId(発行元ID)`で採択
- 可観測性：`issue/accept/release/expire`のログを必ず一行で残す．
- 冪等性：同一`id`の`Issue/Release`は冪等．`InterruptedOnce`はframe内重複抑止

## Arbitration / Priority
* Control-plane(占有の採択)：`severity(Hard) > priority(dsec) > issued_at(asc) > issuerId`
    * `priority/severity`：もし複数のLeaseが同時に来たらどちらを採用すべきか
    * `issuerID`：誰がこのFSMを制御しようとしているか
    * `TTL`：永続的制御ではなく，自然に切れる可能性もある
* Data-plane(内部遷移)：既存`.priority`規則(変更なし)

## Prototype Plan (Vertical Slices)
1) Slice A: `Stun`->Movement=Locked持続，Skill=Interrupted->次フレームNone．`ttl`失効で自然復帰
    * 完了条件：ログに`issue/accept/expire`が出て，在位中はSkillが再起動しない
2) Slice B: `Stun`中に`Root`を受け，切り替え時に`Release(old)->Issue(new)`の順で決定的に入れ替わる．
    * 完了条件：古い終了が先着しても状態が誤って解放されず，ログで入れ替え順が確認できる

## Acceptance Criteria
通ればOKな観測可能条件を3–5点（ログ/テスト観測点で書く）。

## Open Questions
未決事項を箇条書き（将来のADR候補）。


```md
[ ] 不変条件を列挙し、単体テスト/アサートで検証している
[ ] 例外安全レベル（basic/strong/no-throw）をAPIごとに明記
[ ] 所有権と寿命の責務が一本化されている（生成/破棄の入口が一つ）
[ ] 時間源とRNGは注入され、決定性テストが通る
[ ] 遷移判定は純粋、副作用はEffect層に隔離されている
[ ] 同一イベントの重複適用が冪等（重複抑止やバージョン管理あり）
[ ] 競合解決は決定的（優先度→タイブレーク規則を明文化）
[ ] SystemのR/Wセットが明示され、迂回書き込みがない
[ ] リアルタイム予算（最大コスト/反復上限/アロケーション方針）を満たす
[ ] ログ/メトリクス/トレースで可観測性が確保されている
[ ] データ/APIのバージョニングと移行手順がある
```