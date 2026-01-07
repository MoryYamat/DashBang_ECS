# Architecture Decision Record

> [!note] Since: 2026-01-07 -
> 

## もくじ (分野別)
- [Audio](#audio)
- [未分類](#uncategorized)

adr


<h1 id = "audio">Audio</h1>

## Audio の設計と実装について
- **ID**: ADR-20260106-01
- **Status**: Accepted
- **Date**: 2026-01-06
- **Owner**: @
- **Tags**: (Audio)
- **Component**: `Game/Audio/`API + `Engine/Audio/`APIの責務分離
- **Related**:

### Context (必須)
なぜ今これが必要か（1〜2行）

- 共通基盤となるEngine側Auidio実装と、ゲーム特有仕様となるGame側Audio実装の責務の境界をはっきりさせるため

> 決定についてあいまいさが生じた場合、これを確認し、必要であれば更新する


### Decision 
- `Engine/Audio`API(以降Engine側)責務
  - `AudioSystem`: AudioCatalogの情報を利用して、実際にSoundを再生する(実装は`AudioBackend`)
  - `AudioCatalog`: `SoundDef`を実リソース情報として保持 + `SoundID`(正規化ID)によってCRUDの効率化

- `Game/Audio`API(以降Game側)責務
  - `AudioCatalogBuilder`: Engine側の`AudioCatalog`のCRUDを利用し、実データから`AudioCatalog`を作成
  - `

```text
- Engine/Audio


- Game/Audio

```

### Consequences
主な効果・トレードオフ・影響範囲（1〜2行）

### Verification
どう検証するか（ログ/テスト観測点を1行）

### Review
見直し条件 or 期限（例：2025-08-25、または条件）

#### 見直し条件
- 2025-01-07: ツール化に伴い、責務境界が変化する可能性が生じた場合



<h1 id = "uncategorized">未分類</h1>