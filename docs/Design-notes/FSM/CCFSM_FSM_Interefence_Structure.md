# <Design-notes FSMの状態干渉の構造分析・設計>
- **Stats**: Draft
- **Owner**: @you
- **Related ADR**:
- **Related Code/PR**: <Path or #id>

## Problem
- `CCFSM`の実装．直交FSM間の**状態への干渉**の設計・実装
- FSM間の**状態への干渉**の構造的実装のプロトタイプ

## Goals / Non-goals
- Goals: 3–5点（観測可能に書く）
- Non-goals: 今回やらないこと（スコープ外）

## Context & Assumptions
前提・依存・制約（外部API/性能予算/スレッドモデルなど）を箇条書きで。

## Interfaces (Data Contracts)
- Events: `Name{fields...}`（役割を1行ずつ）
- Requests/Replies: `Name{fields...}`
- Side-effects keys: 列挙のみ（型/冪等性の一言）

## Flow / Ordering (1フレームの順序)
`A → B → C` の1行。Queue寿命（frame-bounded等）と重複抑止の条件を1行。

## Invariants
常に守る性質（決定性・冪等・再入安全…）を3点以内。

## Arbitration / Priority
採択規則を1–2行（例：`priority > timestamp > issuerId`）。

## Prototype Plan (Vertical Slices)
1) 薄い縦切りA（完了条件1行）
2) 薄い縦切りB（完了条件1行）

## Acceptance Criteria
通ればOKな観測可能条件を3–5点（ログ/テスト観測点で書く）。

## Open Questions
未決事項を箇条書き（将来のADR候補）。