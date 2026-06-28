# System Pipeline 

ここでは、入力から Gameplay Logic、Rendering / Debug Visualization までの runtime 上の処理の流れを説明する．

## もくじ
- [目的](#目的)
- [全体フロー](#全体フロー)
- [各段階の役割](#各段階の役割)
- [具体例](#具体例)
- [この構造にした理由](#この構造にした理由)
- [現行実装の課題](#現行実装の課題)



## 目的

System Pipeline の設計目的は、  
device input や AI などの入力を直接 Gameplay State に反映するのではなく、  
Intent / State / Resolver / Logic の段階に分けて処理することである.

これによって、入力源の違いを吸収し、  
離散的状態の更新(state-machine)、componentの更新、animation / renderingを独自に設計し、  
整理された順序で実行できるようにすることを目指した．

## 全体フロー

```txt
Raw Input
    ↓
Action Input
    ↓
Requested Intent
    ↓
State / Resolver
    ↓
Resolved Intent
    ↓
Gameplay Logic
    ↓
Animation / Rendering
```

## 各段階の役割

| Stage | Responsibility |
| --- | --- |
| Raw Input | key board / mouse などの device input |
| Action Input | device input を Gameplay Action として抽象化 |
| Requested Intent | Action Input や AI などから Actor / Camera への要求を表現 |
| State / Resolver | 現在状態や制約に基づいて要求を解決 |
| Resolved Intent | 実際に Gameplay Logic に反映する正規化された意図 |
| Gameplay Logic | move、attack、hitbox spawn などを実際に実行 |
| Animation / Rendering | 状態に基づいて Animation と Render Command を生成 |

## 具体例

現行実装でこの Layer 分離によって機能する例として、ユーザ入力によるキャラクターの移動処理について説明する．

### Movement Pipeline
Player Character の移動処理は以下のフローで実現されている．

```txt
Key board / Mouse Input
    ↓
Action Input
    ↓
RequestMovementIntent
    ↓
Movement State / Resolver
    ↓
(Resolved)MovementIntent
    ↓
Movement Logic
    ↓
Velocity / Transform Update
    ↓
Animation Selection
    ↓
Rendering
```

各 Stage では具体的に以下のような入力・処理・出力を持つ

| Stage | Resposibility |
| --- | --- |
| Device Input | W/A/S/D などの物理的な入力状態を持つ |
| Action Input | 物理的な入力とGameplay における意味の対応を持つ |
| RequestedMovementIntent | player が 要求した移動方向を保持する |
| Movement State / Resolver | 現在の Actor の状態や制約に基づいて、要求された移動を実行可能な intent に変換する |
| (Resolved)MovementIntent | Gameplay Logic が使用する正規化済み移動意図を保持する |
| Velocity / Transform Update | velocity に基づいて actor の位置を更新する |
| Animation Selection | 移動方向や状態に基づいて再生する animation clip を選択する |
| Rendering | Transform / Animation pose をもとに描画する |

この処理フローにより、入力処理、状態解決、移動ロジック、Animation Selection を分離しやすくなる．  

そのため、入力方式を keyboard から AI / Network Input に変更しても、  
RequestedMovementIntent 以降の処理を再利用しやすい．

## この構造にした理由
この構造はECS Architecture を前提として、入力処理、状態解決、Gameplay Logic、Animation、Rendering を  
できるだけ独立して設計・変更できるようにするために採用した．

### 1. 入力源の差異を吸収するため
Gameplay Logic に対する入力は、ローカルユーザ入力だけでなく、  
AI や Network Input からも同じ Actor に対して操作要求が発生する可能性がある．  

そのため、device input を直接 Gameplay Logic に反映するのではなく、  
一度`RequestedIntent`として表現する構造にした．  

これにより、入力源によらず、  
以降の State / Resolver / Gameplay Logic へ同じ形式で接続しやすくなる．

### 2. 入力要求と状態制約を分離するため
入力が発生したとしても、その要求が常に実行できるとは限らない．  

例えば、攻撃中、硬直中、移動不能状態などでは、同じ移動入力であっても実行できる動作が変化する．  

そのため、入力要求を直接 movement や animation に反映するのではなく、 State / Resolver を通して解決する構造にした．

これにより、状態遷移や制約判定を個別の処理コードに散逸させず、 State / Resolver 側に集約しやすくなると考えた．

### 3. State / Resolver を差し替え可能な設計にするため
State / Resolver は、入力に対する Gameplay 側の制約やルールを表す層として扱っている．  

この層を Gameplay Logic から分離することで、同じ入力体系に対して異なる状態遷移や制約を適用しやすくなる．

将来的には、 State 定義や遷移条件を Data / Authoring 側へ移すことで、  
挙動やルールの変更をC++の処理コードから切り離しやすくすることを目指している．

### 4. Gameplay Logic / Animation / Rendering を分離するため
Gameplay Logic は、 Actor の移動、攻撃、hitbox spawn などのゲーム状態の更新を行う．  

Animation Selection は、pipeline の過程で得られた状態や intent に基づいて再生する AnimationClip を選択する．  
Rendering は、Transform や Pose などの runtime data をもとに RenderCommand を生成する．

この分離によって、移動ロジックを変更しても描画処理などへ直接影響しにくくなり、  
Animation や Rendering を Gameplay Logic から独立して調整しやすくなる．

## 現行実装の課題

### 1. AI / Network input との統合は今後の検証が必要
現行実装では、主にローカルユーザ入力を対象として pipeline を構成している．  
今後は、AI や Network Input からも同じ Requested Intent の経路へ入力を流せるか検証する必要がある．

### 2. Resolver / State の複雑化や肥大化
Intent / State / Resolver によって入力要求と状態制約を分離できる一方で、  
扱う状態や制約が増えると Resolver が複雑化しやすい．  

そのため、Resolver の責務範囲、State の粒度、複数状態間の優先順位を整理する手法を検討する必要がある．

### 3. Resolver / State 定義のデータ駆動化
現行実装では、State や Resolver の定義の多くを C++ コード上で記述している．  

今後は、状態定義や遷移条件を Data / Authoring 側へ移すことで、  
挙動やルールの変更をより扱いやすくする余地がある．

### 4. System 実行順序の明示化
この pipeline は、 Input、Intent、State、Logic、Animation、Rendering の実行順序に依存している．

今後は、system 間の依存関係や実行順序をより明示的にし、変更時に追いやすい構造にする必要がある．